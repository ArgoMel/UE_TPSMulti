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
	if(!AttackerController||
		! VictimController)
	{
		return;
	}
	ABasePlayerState* attackerPlayerState =Cast<ABasePlayerState>(AttackerController->PlayerState);
	ABasePlayerState* victimPlayerState =Cast<ABasePlayerState>(VictimController->PlayerState);
	ABaseGameState* baseGameState = GetGameState<ABaseGameState>();
	if(attackerPlayerState&&
		attackerPlayerState!= victimPlayerState&&
		baseGameState)
	{
		TArray<ABasePlayerState*> playersCurrentlyInTheLead(baseGameState->TopScoringPlayers);
		
		attackerPlayerState->AddToScore(1.f);
		baseGameState->UpdateTopScore(attackerPlayerState);
		if (baseGameState->TopScoringPlayers.Contains(attackerPlayerState))
		{
			ABaseCharacter* leader=Cast<ABaseCharacter>(attackerPlayerState->GetPawn());
			if (leader)
			{
				leader->MulticastGainedTheLead();
			}
		}

		for (int32 i = 0; i < playersCurrentlyInTheLead.Num(); ++i)
		{
			if (!baseGameState->TopScoringPlayers.Contains(playersCurrentlyInTheLead[i]))
			{
				ABaseCharacter* loser=Cast<ABaseCharacter>(playersCurrentlyInTheLead[i]->GetPawn());
				if (loser)
				{
					loser->MulticastLostTheLead();
				}
			}
		}
	}
	if(victimPlayerState)
	{
		victimPlayerState->AddToDefeats(1);
	}

	if(ElimmedCharacter)
	{
		ElimmedCharacter->Elim(false);
	}

	for (auto it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
	{
		ABasePlayerController* basePlayer=Cast<ABasePlayerController>(*it);
		if (basePlayer&&
			attackerPlayerState&&
			victimPlayerState)
		{
			basePlayer->BroadcastElim(attackerPlayerState,victimPlayerState);
		}
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

void ABaseGameMode::PlayerLeftGame(ABasePlayerState* PlayerLeaving) const
{
	if (PlayerLeaving)
	{
		return;
	}
	ABaseGameState* baseGameState = GetGameState<ABaseGameState>();
	if (baseGameState&&baseGameState->TopScoringPlayers.Contains(PlayerLeaving))
	{
		baseGameState->TopScoringPlayers.Remove(PlayerLeaving);
	}
	ABaseCharacter* characterLeaving=Cast<ABaseCharacter>(PlayerLeaving->GetPawn());
	if (characterLeaving)
	{
		characterLeaving->Elim(true);
	}
}

float ABaseGameMode::CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage)
{
    return 0.0f;
}
