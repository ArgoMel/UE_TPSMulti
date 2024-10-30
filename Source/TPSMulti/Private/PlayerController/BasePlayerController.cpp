// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerController/BasePlayerController.h"
#include "HUD/BaseHUD.h"
#include "HUD/CharacterOverlayWidget.h"
#include "Character/BaseCharacter.h"
//#include "Blaster/GameMode/BlasterGameMode.h"
//#include "Blaster/PlayerState/BlasterPlayerState.h"
//#include "Blaster/HUD/Announcement.h"
#include "Component/CombatComponent.h"
//#include "Blaster/GameState/BlasterGameState.h"
//#include "Blaster/HUD/ReturnToMainMenu.h"
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
}

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void ABasePlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
}

void ABasePlayerController::OnRep_MatchState()
{
	
}

void ABasePlayerController::ServerReportPingStatus_Implementation(bool bHighPing)
{
}

void ABasePlayerController::SetHUDTime()
{
}

void ABasePlayerController::PollInit()
{
}

void ABasePlayerController::ServerRequestServerTime_Implementation(float TimeOfClientRequest)
{
}

void ABasePlayerController::ClientReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
}

void ABasePlayerController::CheckTimeSync(float DeltaTime)
{
}

void ABasePlayerController::ServerCheckMatchState_Implementation()
{
}

void ABasePlayerController::ClientJoinMidgame_Implementation(FName StateOfMatch, float Warmup, float Match, float Cooldown, float StartingTime)
{
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
}

void ABasePlayerController::SetHUDShield(float Shield, float MaxShield)
{
}

void ABasePlayerController::SetHUDScore(float Score)
{
}

void ABasePlayerController::SetHUDDefeats(int32 Defeats)
{
}

void ABasePlayerController::SetHUDWeaponAmmo(int32 Ammo)
{
}

void ABasePlayerController::SetHUDCarriedAmmo(int32 Ammo)
{
}

void ABasePlayerController::SetHUDMatchCountdown(float CountdownTime)
{
}

void ABasePlayerController::SetHUDAnnouncementCountdown(float CountdownTime)
{
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
	return 0.0f;
}

void ABasePlayerController::OnMatchStateSet(FName State, bool bTeamsMatch)
{
}

void ABasePlayerController::HandleMatchHasStarted(bool bTeamsMatch)
{
}

void ABasePlayerController::HandleCooldown()
{
}

void ABasePlayerController::BroadcastElim(APlayerState* Attacker, APlayerState* Victim)
{
}
