// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <TPSMulti/TPSMulti.h>
#include "GameFramework/PlayerState.h"
#include "BasePlayerState.generated.h"

class ABaseCharacter;
class ABasePlayerController;

UCLASS()
class TPSMULTI_API ABasePlayerState : public APlayerState
{
	GENERATED_BODY()
protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	virtual void OnRep_Score() override;

private:
	UPROPERTY()
	ABaseCharacter* Character;
	UPROPERTY()
	ABasePlayerController* Controller;

	UPROPERTY(ReplicatedUsing = OnRep_Defeats)
	int32 Defeats;

	UPROPERTY(ReplicatedUsing = OnRep_Team)
	ETeam Team = ETeam::ET_NoTeam;

private:
	UFUNCTION()
	void OnRep_Team();

public:
	UFUNCTION()
	virtual void OnRep_Defeats();

	void AddToScore(float ScoreAmount);
	void AddToDefeats(int32 DefeatsAmount);

	FORCEINLINE ETeam GetTeam() const { return Team; }
	void SetTeam(ETeam TeamToSet);
};
