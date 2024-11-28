// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BaseGameMode.generated.h"

class ABaseCharacter;
class ABasePlayerController;
class ABasePlayerState;

namespace MatchState
{
	// Match duration has been reached. Display winner and begin cooldown timer.
	extern TPSMULTI_API const FName Cooldown; 
}

UCLASS()
class TPSMULTI_API ABaseGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ABaseGameMode();
protected:
	virtual void BeginPlay() override;
	virtual void OnMatchStateSet() override;
	virtual void Tick(float DeltaTime) override;

private:
	float CountdownTime = 0.f;

public:
	UPROPERTY(EditDefaultsOnly)
	float WarmupTime = 10.f;

	UPROPERTY(EditDefaultsOnly)
	float MatchTime = 120.f;

	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 10.f;

	float LevelStartingTime = 0.f;

	bool bTeamsMatch = false;

public:
	virtual void PlayerEliminated(ABaseCharacter* ElimmedCharacter, ABasePlayerController* VictimController, ABasePlayerController* AttackerController);
	virtual void RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController);
	void PlayerLeftGame(ABasePlayerState* PlayerLeaving) const;
	virtual float CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage);

	FORCEINLINE float GetCountdownTime() const { return CountdownTime; }
};
