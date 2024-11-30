// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGameMode.h"
#include "TeamGameMode.generated.h"

UCLASS()
class TPSMULTI_API ATeamGameMode : public ABaseGameMode
{
	GENERATED_BODY()
public:
	ATeamGameMode();
protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

protected:
	virtual float CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage) override;
	virtual void PlayerEliminated(ABaseCharacter* ElimmedCharacter, ABasePlayerController* VictimController, ABasePlayerController* AttackerController) override;
	virtual void HandleMatchHasStarted() override;
};
