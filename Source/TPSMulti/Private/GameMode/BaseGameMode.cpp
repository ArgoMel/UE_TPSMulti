// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/BaseGameMode.h"
#include "Character/BaseCharacter.h"
#include "PlayerController/BasePlayerController.h"
#include "PlayerState/BasePlayerState.h"
#include "GameState/BaseGameState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

namespace MatchState
{
	const FName Cooldown = FName("Cooldown");
}

ABaseGameMode::ABaseGameMode()
{
	bDelayedStart = true;
}

void ABaseGameMode::BeginPlay()
{
	Super::BeginPlay();
	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

void ABaseGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();
	for (FConstPlayerControllerIterator iter = GetWorld()->GetPlayerControllerIterator(); iter;++iter)
	{
		ABasePlayerController* basePlayer = Cast<ABasePlayerController>(*iter);
		if(basePlayer)
		{
			basePlayer->OnMatchStateSet(MatchState);
		}
	}
}

void ABaseGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(MatchState== MatchState::WaitingToStart)
	{
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds()+LevelStartingTime;
		if(CountdownTime<=0.f)
		{
			StartMatch();
		}
	}
	else if (MatchState == MatchState::InProgress)
	{
		CountdownTime = WarmupTime +MatchTime- GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			SetMatchState(MatchState::Cooldown);
		}
	}
	else if (MatchState == MatchState::Cooldown)
	{
		CountdownTime = CooldownTime + WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			RestartGame();
		}
	}
}

void ABaseGameMode::PlayerEliminated(ABaseCharacter* ElimmedCharacter, ABasePlayerController* VictimController, ABasePlayerController* AttackerController)
{
	if(AttackerController&& VictimController)
	{
		ABasePlayerState* attackerPlayerState =Cast<ABasePlayerState>(AttackerController->PlayerState);
		ABasePlayerState* victimPlayerState =Cast<ABasePlayerState>(VictimController->PlayerState);
		ABaseGameState* baseGameState = GetGameState<ABaseGameState>();

		if(attackerPlayerState&&
			attackerPlayerState!= victimPlayerState&&
			baseGameState)
		{
			attackerPlayerState->AddToScore(1.f);
			baseGameState->UpdateTopScore(attackerPlayerState);

		}
		if(victimPlayerState)
		{
			victimPlayerState->AddToDefeats(1);
		}
	}

	if(ElimmedCharacter)
	{
		ElimmedCharacter->Elim(false);
	}
}

void ABaseGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if(ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	if(ElimmedController)
	{
		TArray<AActor*> playerStarts;
		UGameplayStatics::GetAllActorsOfClass(this,APlayerStart::StaticClass(), playerStarts);
		int32 selection = FMath::RandRange(0, playerStarts.Num()-1);
		RestartPlayerAtPlayerStart(ElimmedController, playerStarts[selection]);
	}
}

void ABaseGameMode::PlayerLeftGame(ABasePlayerState* PlayerLeaving)
{
}

float ABaseGameMode::CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage)
{
    return 0.0f;
}
