// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BaseGameState.generated.h"

class ABasePlayerState;

UCLASS()
class TPSMULTI_API ABaseGameState : public AGameState
{
	GENERATED_BODY()
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	float TopScore = 0.f;

public:
	UPROPERTY(Replicated)
	TArray<ABasePlayerState*> TopScoringPlayers;

	TArray<ABasePlayerState*> RedTeam;
	TArray<ABasePlayerState*> BlueTeam;

	UPROPERTY(ReplicatedUsing = OnRep_RedTeamScore)
	float RedTeamScore = 0.f;

	UPROPERTY(ReplicatedUsing = OnRep_BlueTeamScore)
	float BlueTeamScore = 0.f;

public:
	void UpdateTopScore(ABasePlayerState* ScoringPlayer);

	// Teams
	void RedTeamScores();
	void BlueTeamScores();

	UFUNCTION()
	void OnRep_RedTeamScore();

	UFUNCTION()
	void OnRep_BlueTeamScore();
};
