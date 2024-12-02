// Fill out your copyright notice in the Description page of Project Settings.

#include "GameState/BaseGameState.h"
#include "PlayerState/BasePlayerState.h"
#include "Net/UnrealNetwork.h"
#include "PlayerController/BasePlayerController.h"

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
	++RedTeamScore;
	ABasePlayerController* basePlayer =Cast<ABasePlayerController>(GetWorld()->GetFirstPlayerController());
	if (basePlayer)
	{
		basePlayer->SetHUDRedTeamScore(RedTeamScore);
	}
}

void ABaseGameState::BlueTeamScores()
{
	++BlueTeamScore;
	ABasePlayerController* basePlayer =Cast<ABasePlayerController>(GetWorld()->GetFirstPlayerController());
	if (basePlayer)
	{
		basePlayer->SetHUDBlueTeamScore(BlueTeamScore);
	}
}

void ABaseGameState::OnRep_RedTeamScore() const
{
	ABasePlayerController* basePlayer =Cast<ABasePlayerController>(GetWorld()->GetFirstPlayerController());
	if (basePlayer)
	{
		basePlayer->SetHUDRedTeamScore(RedTeamScore);
	}
}

void ABaseGameState::OnRep_BlueTeamScore() const
{
	ABasePlayerController* basePlayer =Cast<ABasePlayerController>(GetWorld()->GetFirstPlayerController());
	if (basePlayer)
	{
		basePlayer->SetHUDBlueTeamScore(BlueTeamScore);
	}
}
