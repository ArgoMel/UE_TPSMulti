// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerController/BasePlayerController.h"
#include "HUD/BaseHUD.h"
#include "HUD/CharacterOverlayWidget.h"
#include "Character/BaseCharacter.h"
#include "GameMode/BaseGameMode.h"
#include "PlayerState/BasePlayerState.h"
#include "HUD/AnnouncementWidget.h"
#include "Component/CombatComponent.h"
#include "GameState/BaseGameState.h"
//#include "HUD/ReturnToMainMenu.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "GameFramework/PlayerState.h"

void ABasePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, MatchState);
	DOREPLIFETIME(ThisClass, bShowTeamScores);
}

void ABasePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(InPawn);
	if (baseCharacter)
	{
		SetHUDHealth(baseCharacter->GetHealth(), baseCharacter->GetMaxHealth());
	}
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();
	BaseHUD = Cast<ABaseHUD>(GetHUD());
	ServerCheckMatchState();
}

void ABasePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetHUDTime();
	CheckTimeSync(DeltaTime);
	PollInit();
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
				SetHUDHealth(HUDHealth,HUDMaxHealth);
				SetHUDScore(HUDScore);
				SetHUDDefeats(HUDDefeats);
			}
		}
	}
}

void ABasePlayerController::ServerRequestServerTime_Implementation(float TimeOfClientRequest)
{
	float serverTimeOfReceipt = GetWorld()->GetTimeSeconds();
	ClientReportServerTime(TimeOfClientRequest, serverTimeOfReceipt);
}

void ABasePlayerController::ClientReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
	float roundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	float curServerTime = TimeServerReceivedClientRequest+(0.5f*roundTripTime);
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
	ABaseGameMode* gameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(this));
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
}

void ABasePlayerController::StopHighPingWarning()
{
}

void ABasePlayerController::CheckPing(float DeltaTime)
{
}

void ABasePlayerController::ShowReturnToMainMenu()
{
}

void ABasePlayerController::ClientElimAnnouncement_Implementation(APlayerState* Attacker, APlayerState* Victim)
{
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
	if(!BaseHUD)
	{
		BaseHUD = Cast<ABaseHUD>(GetHUD());
	}
	bool bHUDValid = BaseHUD &&
		BaseHUD->CharacterOverlay &&
		BaseHUD->CharacterOverlay->HealthBar &&
		BaseHUD->CharacterOverlay->HealthText;
	if(bHUDValid)
	{
		const float healthPercent = Health / MaxHealth;
		BaseHUD->CharacterOverlay->HealthBar->SetPercent(healthPercent);
		FString healthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		BaseHUD->CharacterOverlay->HealthText->SetText(FText::FromString(healthText));
	}
	else
	{
		bInitializeHealth = true;
		HUDHealth = Health;
		HUDMaxHealth = MaxHealth;
	}
}

void ABasePlayerController::SetHUDShield(float Shield, float MaxShield)
{
}

void ABasePlayerController::SetHUDScore(float Score)
{
	if (!BaseHUD)
	{
		BaseHUD = Cast<ABaseHUD>(GetHUD());
	}
	bool bHUDValid = BaseHUD &&
		BaseHUD->CharacterOverlay &&
		BaseHUD->CharacterOverlay->ScoreAmount;
	if (bHUDValid)
	{
		FString scoreText = FString::Printf(TEXT("%d"), FMath::CeilToInt(Score));
		BaseHUD->CharacterOverlay->ScoreAmount->SetText(FText::FromString(scoreText));
	}
	else
	{
		bInitializeScore = true;
		HUDScore = Score;
	}
}

void ABasePlayerController::SetHUDDefeats(int32 Defeats)
{
	if (!BaseHUD)
	{
		BaseHUD = Cast<ABaseHUD>(GetHUD());
	}
	bool bHUDValid = BaseHUD &&
		BaseHUD->CharacterOverlay &&
		BaseHUD->CharacterOverlay->DefeatsAmount;
	if (bHUDValid)
	{
		FString defeatsText = FString::Printf(TEXT("%d"), Defeats);
		BaseHUD->CharacterOverlay->DefeatsAmount->SetText(FText::FromString(defeatsText));
	}
	else
	{
		bInitializeDefeats = true;
		HUDDefeats = Defeats;
	}
}

void ABasePlayerController::SetHUDWeaponAmmo(int32 Ammo, EWeaponType WeaponType)
{
	if (!BaseHUD)
	{
		BaseHUD = Cast<ABaseHUD>(GetHUD());
	}
	bool bHUDValid = BaseHUD &&
		BaseHUD->CharacterOverlay &&
		BaseHUD->CharacterOverlay->WeaponAmmoAmount&&
		BaseHUD->CharacterOverlay->WeaponTypeText;
	if (bHUDValid)
	{
		BaseHUD->CharacterOverlay->SetAmmoUI(Ammo != NO_WEAPON);
		FString ammoText = FString::Printf(TEXT("%d"), Ammo);
		BaseHUD->CharacterOverlay->WeaponAmmoAmount->SetText(FText::FromString(ammoText));
		BaseHUD->CharacterOverlay->WeaponTypeText->SetText(UEnum::GetDisplayValueAsText(WeaponType));
	}
}

void ABasePlayerController::SetHUDCarriedAmmo(int32 Ammo)
{
	if (!BaseHUD)
	{
		BaseHUD = Cast<ABaseHUD>(GetHUD());
	}
	bool bHUDValid = BaseHUD &&
		BaseHUD->CharacterOverlay &&
		BaseHUD->CharacterOverlay->CarriedAmmoAmount;
	if (bHUDValid)
	{
		BaseHUD->CharacterOverlay->SetAmmoUI(Ammo != NO_WEAPON);
		FString ammoText = FString::Printf(TEXT("%d"), Ammo);
		BaseHUD->CharacterOverlay->CarriedAmmoAmount->SetText(FText::FromString(ammoText));
	}
}

void ABasePlayerController::SetHUDMatchCountdown(float CountdownTime)
{
	if (!BaseHUD)
	{
		BaseHUD = Cast<ABaseHUD>(GetHUD());
	}
	bool bHUDValid = BaseHUD &&
		BaseHUD->CharacterOverlay &&
		BaseHUD->CharacterOverlay->MatchCountdownText;
	if (bHUDValid)
	{
		if (CountdownTime < 0.f)
		{
			BaseHUD->CharacterOverlay->MatchCountdownText->SetText(FText::GetEmpty());
			return;
		}

		int32 minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 seconds = CountdownTime- minutes*60;
		FString countdownText = FString::Printf(TEXT("%02d:%02d"), minutes, seconds);
		BaseHUD->CharacterOverlay->MatchCountdownText->SetText(FText::FromString(countdownText));

		BaseHUD->CharacterOverlay->PlayBlinkTextAnim(CountdownTime < 30.f);
	}
}

void ABasePlayerController::SetHUDAnnouncementCountdown(float CountdownTime)
{
	if (!BaseHUD)
	{
		BaseHUD = Cast<ABaseHUD>(GetHUD());
	}
	bool bHUDValid = BaseHUD &&
		BaseHUD->Announcement &&
		BaseHUD->Announcement->WarmupTime;
	if (bHUDValid)
	{
		if(CountdownTime<0.f)
		{
			BaseHUD->Announcement->WarmupTime->SetText(FText::GetEmpty());
			return;
		}

		int32 minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 seconds = CountdownTime - minutes * 60;
		FString countdownText = FString::Printf(TEXT("%02d:%02d"), minutes, seconds);
		BaseHUD->Announcement->WarmupTime->SetText(FText::FromString(countdownText));
	}
}

void ABasePlayerController::SetHUDGrenades(int32 Grenades)
{
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
	if (!BaseHUD)
	{
		BaseHUD = Cast<ABaseHUD>(GetHUD());
	}
	if (BaseHUD)
	{
		BaseHUD->AddCharacterOverlay();
		if (BaseHUD->Announcement)
		{
			BaseHUD->Announcement->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void ABasePlayerController::HandleCooldown()
{
	if (!BaseHUD)
	{
		BaseHUD = Cast<ABaseHUD>(GetHUD());
	}
	if (BaseHUD)
	{
		BaseHUD->CharacterOverlay->RemoveFromParent();
		if (BaseHUD->Announcement&&
			BaseHUD->Announcement->AnnouncementText&&
			BaseHUD->Announcement->InfoText)
		{
			BaseHUD->Announcement->SetVisibility(ESlateVisibility::HitTestInvisible);
			FString announcementText(TEXT("New Match Starts In: "));
			BaseHUD->Announcement->AnnouncementText->SetText(FText::FromString(announcementText));

			ABaseGameState* baseGameState = Cast<ABaseGameState>(UGameplayStatics::GetGameState(this));
			ABasePlayerState* basePlayerState = GetPlayerState<ABasePlayerState>();
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
					for(auto& tiedPlayer: topPlayers)
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
}
