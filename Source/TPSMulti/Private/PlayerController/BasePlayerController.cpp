// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerController/BasePlayerController.h"
#include "HUD/BaseHUD.h"
#include "HUD/CharacterOverlayWidget.h"
#include "Character/BaseCharacter.h"
#include "GameMode/BaseGameMode.h"
#include "PlayerState/BasePlayerState.h"
#include "HUD/AnnouncementWidget.h"
#include "Component/CombatComponent.h"
#include "Weapon/Weapon.h"
#include "GameState/BaseGameState.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "GameFramework/PlayerState.h"
#include "HUD/ReturnToMainMenuWidget.h"

void ABasePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, MatchState);
	DOREPLIFETIME(ThisClass, bShowTeamScores);
}

void ABasePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	const ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(InPawn);
	if (baseCharacter)
	{
		SetHUDHealth(baseCharacter->GetHealth(), baseCharacter->GetMaxHealth());
	}
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();
	GetBaseHUD();
	ServerCheckMatchState();
}

void ABasePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetHUDTime();
	CheckTimeSync(DeltaTime);
	PollInit();
	CheckPing(DeltaTime);
}

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void ABasePlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	if(IsLocalController())
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
	}
}

void ABasePlayerController::OnRep_MatchState()
{
	if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
	else if (MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
}

void ABasePlayerController::ServerReportPingStatus_Implementation(bool bHighPing)
{
	HighPingDelegate.Broadcast(bHighPing);
}

void ABasePlayerController::SetHUDTime()
{
	float timeLeft = 0.f;
	if (MatchState == MatchState::WaitingToStart)
	{
		timeLeft=WarmupTime - GetServerTime()+LevelStartingTime;
	}
	else if (MatchState == MatchState::InProgress)
	{
		timeLeft= WarmupTime+MatchTime - GetServerTime()+ LevelStartingTime;
	}
	else if (MatchState == MatchState::Cooldown)
	{
		timeLeft = CooldownTime+WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
	}
	uint32 seconsLeft = FMath::CeilToInt(timeLeft);

	if(HasAuthority())
	{
		if(!BaseGameMode)
		{
			BaseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(this));
		}
		if(BaseGameMode)
		{
			seconsLeft = FMath::CeilToInt(BaseGameMode->GetCountdownTime()+LevelStartingTime);
		}
	}

	if(CountdownInt!=seconsLeft)
	{
		if(MatchState==MatchState::WaitingToStart||
			MatchState==MatchState::Cooldown)
		{
			SetHUDAnnouncementCountdown(timeLeft);
		}
		else if (MatchState == MatchState::InProgress)
		{
			SetHUDMatchCountdown(timeLeft);
		}
	}

	CountdownInt = seconsLeft;
}

void ABasePlayerController::PollInit()
{
	if(!CharacterOverlay)
	{
		if (BaseHUD&&
			BaseHUD->CharacterOverlay)
		{
			CharacterOverlay = BaseHUD->CharacterOverlay;
			if(CharacterOverlay)
			{
				const ABasePlayerState* basePlayerState = GetPlayerState<ABasePlayerState>();
				if(basePlayerState)
				{
					if (bInitializeScore)
					{
						SetHUDScore(basePlayerState->GetScore());
					}
					if (bInitializeDefeats)
					{
						SetHUDDefeats(basePlayerState->GetDefeat());
					}
				}

				const ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(GetPawn());
				if (baseCharacter)
				{
					if (bInitializeHealth)
					{
						SetHUDHealth(baseCharacter->GetHealth(), baseCharacter->GetMaxHealth());
					}
					if (bInitializeShield)
					{
						SetHUDShield(baseCharacter->GetShield(), baseCharacter->GetMaxShield());
					}

					if (baseCharacter->GetCombat())
					{
						if (bInitializeWeaponAmmo &&
							baseCharacter->GetCombat()->GetEquippedWeapon())
						{
							SetHUDWeaponAmmo(baseCharacter->GetCombat()->GetEquippedWeapon()->GetAmmo(), baseCharacter->GetCombat()->GetEquippedWeapon()->GetWeaponType());
						}
						if (bInitializeCarriedAmmo)
						{
							SetHUDCarriedAmmo(baseCharacter->GetCombat()->GetCarriedAmmo());
						}
						if (bInitializeGrenades)
						{
							SetHUDGrenades(baseCharacter->GetCombat()->GetGrenades());
						}
					}
				}
			}
		}
	}
}

void ABasePlayerController::ServerRequestServerTime_Implementation(float TimeOfClientRequest)
{
	const float serverTimeOfReceipt = GetWorld()->GetTimeSeconds();
	ClientReportServerTime(TimeOfClientRequest, serverTimeOfReceipt);
}

void ABasePlayerController::ClientReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
	const float roundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	SingleTripTime=0.5f*roundTripTime;
	const float curServerTime = TimeServerReceivedClientRequest+SingleTripTime;
	ClientServerDelta = curServerTime- GetWorld()->GetTimeSeconds();
}

void ABasePlayerController::CheckTimeSync(float DeltaTime)
{
	TimeSyncRunningTime += DeltaTime;
	if (IsLocalController() &&
		TimeSyncRunningTime > TimeSyncFrequency)
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;
	}
}

void ABasePlayerController::ServerCheckMatchState_Implementation()
{
	const ABaseGameMode* gameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(this));
	if(gameMode)
	{
		WarmupTime = gameMode->WarmupTime;
		MatchTime = gameMode->MatchTime;
		CooldownTime = gameMode->CooldownTime;
		LevelStartingTime = gameMode->LevelStartingTime;
		MatchState = gameMode->GetMatchState();
		ClientJoinMidgame(MatchState, WarmupTime, MatchTime, CooldownTime,LevelStartingTime);
	}
}

void ABasePlayerController::ClientJoinMidgame_Implementation(FName StateOfMatch, float Warmup, float Match, float Cooldown, float StartingTime)
{
	WarmupTime = Warmup;
	MatchTime = Match;
	CooldownTime = Cooldown;
	LevelStartingTime = StartingTime;
	MatchState = StateOfMatch;
	OnMatchStateSet(MatchState);
	if (BaseHUD &&
		MatchState == MatchState::WaitingToStart)
	{
		BaseHUD->AddAnnouncement();
	}
}

void ABasePlayerController::HighPingWarning()
{
	if (GetBaseHUD() &&
		BaseHUD->CharacterOverlay &&
		BaseHUD->CharacterOverlay->HighPingImage&&
		BaseHUD->CharacterOverlay->HighPingAnimation)
	{
		BaseHUD->CharacterOverlay->HighPingImage->SetOpacity(1.f);
		BaseHUD->CharacterOverlay->PlayAnimation(BaseHUD->CharacterOverlay->HighPingAnimation,0.f,3,EUMGSequencePlayMode::PingPong);
	}
}

void ABasePlayerController::StopHighPingWarning()
{
	if (GetBaseHUD() &&
		BaseHUD->CharacterOverlay &&
		BaseHUD->CharacterOverlay->HighPingImage &&
		BaseHUD->CharacterOverlay->HighPingAnimation)
	{
		BaseHUD->CharacterOverlay->HighPingImage->SetOpacity(0.f);
		if(BaseHUD->CharacterOverlay->IsAnimationPlaying(BaseHUD->CharacterOverlay->HighPingAnimation))
		{
			BaseHUD->CharacterOverlay->StopAnimation(BaseHUD->CharacterOverlay->HighPingAnimation);
		}
	}
}

void ABasePlayerController::CheckPing(float DeltaTime)
{
	HighPingRunningTime += DeltaTime;
	if (HighPingRunningTime > CheckPingFrequency)
	{
		if (!PlayerState)
		{
			PlayerState = GetPlayerState<APlayerState>();
		}
		if (PlayerState)
		{
			if (PlayerState->GetPingInMilliseconds() * 4.f > HighPingThreshold)
			{
				HighPingWarning();
				PingAnimationRunningTime = 0.f;
				ServerReportPingStatus(true);
			}
			else
			{
				ServerReportPingStatus(false);
			}
		}
		HighPingRunningTime = 0.f;
	}

	if (BaseHUD &&
		BaseHUD->CharacterOverlay &&
		BaseHUD->CharacterOverlay->HighPingAnimation &&
		BaseHUD->CharacterOverlay->IsAnimationPlaying(BaseHUD->CharacterOverlay->HighPingAnimation))
	{
		PingAnimationRunningTime += DeltaTime;
		if (PingAnimationRunningTime > HighPingDuration)
		{
			StopHighPingWarning();
		}
	}
}

void ABasePlayerController::ShowReturnToMainMenu()
{
	if (!ReturnToMainMenuWidget)
	{
		return;
	}
	if (!ReturnToMainMenu)
	{
		ReturnToMainMenu=CreateWidget<UReturnToMainMenuWidget>(this,ReturnToMainMenuWidget);
	}
	if (ReturnToMainMenu)
	{
		bReturnToMainMenuOpen=!bReturnToMainMenuOpen;
		if (bReturnToMainMenuOpen)
		{
			ReturnToMainMenu->MenuSetup();
		}
		else
		{
			ReturnToMainMenu->MenuTearDown();
		}
	}
}

void ABasePlayerController::ClientElimAnnouncement_Implementation(APlayerState* Attacker, APlayerState* Victim)
{
	const APlayerState* self=GetPlayerState<APlayerState>();
	if (self&&
		Victim&&
		Attacker)
	{
		if (GetBaseHUD())
		{
			if (Attacker==self&&Victim!=self)
			{
				BaseHUD->AddElimAnnouncement(TEXT("You"),Victim->GetPlayerName());
				return;
			}
			if (Victim==self&&Attacker!=self)
			{
				BaseHUD->AddElimAnnouncement(Attacker->GetPlayerName(),TEXT("You"));
				return;
			}
			if (Attacker==Victim&&Attacker==self)
			{
				BaseHUD->AddElimAnnouncement(TEXT("You"),TEXT("Yourself"));
				return;
			}
			if (Attacker==Victim&&Attacker!=self)
			{
				BaseHUD->AddElimAnnouncement(Attacker->GetPlayerName(),TEXT("Themselves"));
				return;
			}
			BaseHUD->AddElimAnnouncement(Attacker->GetPlayerName(),Victim->GetPlayerName());
		}
	}
}

void ABasePlayerController::OnRep_ShowTeamScores()
{
}

FString ABasePlayerController::GetInfoText(const TArray<class ABlasterPlayerState*>& Players)
{
	return FString();
}

FString ABasePlayerController::GetTeamsInfoText(ABlasterGameState* BlasterGameState)
{
	return FString();
}

void ABasePlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	if(GetBaseHUD() &&
		BaseHUD->CharacterOverlay &&
		BaseHUD->CharacterOverlay->HealthBar &&
		BaseHUD->CharacterOverlay->HealthText)
	{
		const float healthPercent = Health / MaxHealth;
		BaseHUD->CharacterOverlay->HealthBar->SetPercent(healthPercent);
		const FString healthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		BaseHUD->CharacterOverlay->HealthText->SetText(FText::FromString(healthText));
	}
	else
	{
		bInitializeHealth = true;
	}
}

void ABasePlayerController::SetHUDShield(float Shield, float MaxShield)
{
	if (GetBaseHUD() &&
		BaseHUD->CharacterOverlay &&
		BaseHUD->CharacterOverlay->ShieldBar &&
		BaseHUD->CharacterOverlay->ShieldText)
	{
		const float shieldPercent = Shield / MaxShield;
		BaseHUD->CharacterOverlay->ShieldBar->SetPercent(shieldPercent);
		const FString shieldText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Shield), FMath::CeilToInt(MaxShield));
		BaseHUD->CharacterOverlay->ShieldText->SetText(FText::FromString(shieldText));
	}
	else
	{
		bInitializeShield = true;
	}
}

void ABasePlayerController::SetHUDScore(float Score)
{
	const bool bHUDValid = GetBaseHUD() &&
		BaseHUD->CharacterOverlay &&
		BaseHUD->CharacterOverlay->ScoreAmount;
	if (bHUDValid)
	{
		const FString scoreText = FString::Printf(TEXT("%d"), FMath::CeilToInt(Score));
		BaseHUD->CharacterOverlay->ScoreAmount->SetText(FText::FromString(scoreText));
	}
	else
	{
		bInitializeScore = true;
	}
}

void ABasePlayerController::SetHUDDefeats(int32 Defeats)
{
	const bool bHUDValid = GetBaseHUD() &&
		BaseHUD->CharacterOverlay &&
		BaseHUD->CharacterOverlay->DefeatsAmount;
	if (bHUDValid)
	{
		const FString defeatsText = FString::Printf(TEXT("%d"), Defeats);
		BaseHUD->CharacterOverlay->DefeatsAmount->SetText(FText::FromString(defeatsText));
	}
	else
	{
		bInitializeDefeats = true;
	}
}

void ABasePlayerController::SetHUDWeaponAmmo(int32 Ammo, EWeaponType WeaponType)
{
	if (GetBaseHUD() &&
		BaseHUD->CharacterOverlay &&
		BaseHUD->CharacterOverlay->WeaponAmmoAmount &&
		BaseHUD->CharacterOverlay->WeaponTypeText)
	{
		BaseHUD->CharacterOverlay->SetAmmoUI(Ammo != NO_WEAPON);
		const FString ammoText = FString::Printf(TEXT("%d"), Ammo);
		BaseHUD->CharacterOverlay->WeaponAmmoAmount->SetText(FText::FromString(ammoText));
		BaseHUD->CharacterOverlay->WeaponTypeText->SetText(UEnum::GetDisplayValueAsText(WeaponType));
	}
	else
	{
		bInitializeWeaponAmmo = true;
	}
}

void ABasePlayerController::SetHUDCarriedAmmo(int32 Ammo)
{
	if (GetBaseHUD() &&
		BaseHUD->CharacterOverlay &&
		BaseHUD->CharacterOverlay->CarriedAmmoAmount)
	{
		BaseHUD->CharacterOverlay->SetAmmoUI(Ammo != NO_WEAPON);
		FString ammoText = FString::Printf(TEXT("%d"), Ammo);
		if(Ammo==INF_AMMO)
		{
			ammoText = TEXT("INF");
		}
		BaseHUD->CharacterOverlay->CarriedAmmoAmount->SetText(FText::FromString(ammoText));
	}
	else
	{
		bInitializeCarriedAmmo = true;
	}
}

void ABasePlayerController::SetHUDMatchCountdown(float CountdownTime)
{
	const bool bHUDValid = GetBaseHUD() &&
		BaseHUD->CharacterOverlay &&
		BaseHUD->CharacterOverlay->MatchCountdownText;
	if (bHUDValid)
	{
		if (CountdownTime < 0.f)
		{
			BaseHUD->CharacterOverlay->MatchCountdownText->SetText(FText::GetEmpty());
			return;
		}

		const int32 minutes = FMath::FloorToInt(CountdownTime / 60.f);
		const int32 seconds = CountdownTime- minutes*60;
		const FString countdownText = FString::Printf(TEXT("%02d:%02d"), minutes, seconds);
		BaseHUD->CharacterOverlay->MatchCountdownText->SetText(FText::FromString(countdownText));

		BaseHUD->CharacterOverlay->PlayBlinkTextAnim(CountdownTime < 30.f);
	}
}

void ABasePlayerController::SetHUDAnnouncementCountdown(float CountdownTime)
{
	const bool bHUDValid = GetBaseHUD() &&
		BaseHUD->Announcement &&
		BaseHUD->Announcement->WarmupTime;
	if (bHUDValid)
	{
		if(CountdownTime<0.f)
		{
			BaseHUD->Announcement->WarmupTime->SetText(FText::GetEmpty());
			return;
		}

		const int32 minutes = FMath::FloorToInt(CountdownTime / 60.f);
		const int32 seconds = CountdownTime - minutes * 60;
		const FString countdownText = FString::Printf(TEXT("%02d:%02d"), minutes, seconds);
		BaseHUD->Announcement->WarmupTime->SetText(FText::FromString(countdownText));
	}
}

void ABasePlayerController::SetHUDGrenades(int32 Grenades)
{
	if (GetBaseHUD() &&
		BaseHUD->CharacterOverlay &&
		BaseHUD->CharacterOverlay->GrenadesText)
	{
		FString grenadesText = FString::Printf(TEXT("%d"), Grenades);
		if (Grenades == INF_AMMO)
		{
			grenadesText = TEXT("INF");
		}
		BaseHUD->CharacterOverlay->GrenadesText->SetText(FText::FromString(grenadesText));
	}
	else
	{
		bInitializeGrenades = true;
	}
}

void ABasePlayerController::HideTeamScores()
{
}

void ABasePlayerController::InitTeamScores()
{
}

void ABasePlayerController::SetHUDRedTeamScore(int32 RedScore)
{
}

void ABasePlayerController::SetHUDBlueTeamScore(int32 BlueScore)
{
}

float ABasePlayerController::GetServerTime()
{
	return GetWorld()->GetTimeSeconds()+ClientServerDelta;
}

void ABasePlayerController::OnMatchStateSet(FName State, bool bTeamsMatch)
{
	MatchState = State;
	if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted(bTeamsMatch);
	}
	else if(MatchState==MatchState::Cooldown)
	{
		HandleCooldown();
	}
}

void ABasePlayerController::HandleMatchHasStarted(bool bTeamsMatch)
{
	if (GetBaseHUD())
	{
		if(!BaseHUD->CharacterOverlay)
		{
			BaseHUD->AddCharacterOverlay();
		}
		if (BaseHUD->Announcement)
		{
			BaseHUD->Announcement->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void ABasePlayerController::HandleCooldown()
{
	if (GetBaseHUD())
	{
		BaseHUD->CharacterOverlay->RemoveFromParent();
		if (BaseHUD->Announcement&&
			BaseHUD->Announcement->AnnouncementText&&
			BaseHUD->Announcement->InfoText)
		{
			BaseHUD->Announcement->SetVisibility(ESlateVisibility::HitTestInvisible);
			const FString announcementText(TEXT("New Match Starts In: "));
			BaseHUD->Announcement->AnnouncementText->SetText(FText::FromString(announcementText));

			const ABaseGameState* baseGameState = Cast<ABaseGameState>(UGameplayStatics::GetGameState(this));
			const ABasePlayerState* basePlayerState = GetPlayerState<ABasePlayerState>();
			if(baseGameState&&
				basePlayerState)
			{
				TArray<ABasePlayerState*> topPlayers = baseGameState->TopScoringPlayers;
				FString infoTextStr;
				if(topPlayers.IsEmpty())
				{
					infoTextStr = TEXT("There is no winner.");
				}
				else if(topPlayers.Num()==1&& 
					topPlayers[0]== basePlayerState)
				{
					infoTextStr = TEXT("You are the winner!");
				}
				else if (topPlayers.Num() == 1 &&
					topPlayers[0] != basePlayerState)
				{
					infoTextStr = FString::Printf(TEXT("Winner: \n%s"), *topPlayers[0]->GetPlayerName());
				}
				else if (topPlayers.Num() >1)
				{
					infoTextStr = TEXT("Players tied for the win:\n");
					for(const auto& tiedPlayer: topPlayers)
					{
						infoTextStr.Append(FString::Printf(TEXT("%s\n"), *tiedPlayer->GetPlayerName()));
					}
				}
				BaseHUD->Announcement->InfoText->SetText(FText::FromString(infoTextStr));
			}
		}
	}
	ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(GetPawn());
	if(baseCharacter&&
		baseCharacter->GetCombat())
	{
		baseCharacter->bDisableGameplay = true;
		baseCharacter->GetCombat()->FireButtonPressed(false);
	}
}

void ABasePlayerController::BroadcastElim(APlayerState* Attacker, APlayerState* Victim)
{
	ClientElimAnnouncement(Attacker, Victim);	
}

ABaseHUD* ABasePlayerController::GetBaseHUD()
{
	if (!BaseHUD)
	{
		BaseHUD = Cast<ABaseHUD>(GetHUD());
	}
	return BaseHUD;
}
