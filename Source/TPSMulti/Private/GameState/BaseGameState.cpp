// Fill out your copyright notice in the Description page of Project Settings.

#include "GameState/BaseGameState.h"
#include "PlayerState/BasePlayerState.h"
#include "PlayerController/BasePlayerController.h"
#include "Net/UnrealNetwork.h"

void ABaseGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, TopScoringPlayers);
	DOREPLIFETIME(ThisClass, RedTeamScore);
	DOREPLIFETIME(ThisClass, BlueTeamScore);
}

void ABaseGameState::UpdateTopScore(ABasePlayerState* ScoringPlayer)
{
	if(TopScoringPlayers.IsEmpty())
	{
		TopScoringPlayers.Add(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
	else if(ScoringPlayer->GetScore()== TopScore)
	{
		TopScoringPlayers.AddUnique(ScoringPlayer);
	}
	else if (ScoringPlayer->GetScore() > TopScore)
	{
		TopScoringPlayers.Empty();
		TopScoringPlayers.Add(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
}

void ABaseGameState::RedTeamScores()
{
}

void ABaseGameState::BlueTeamScores()
{
}

void ABaseGameState::OnRep_RedTeamScore()
{
}

void ABaseGameState::OnRep_BlueTeamScore()
{
}
