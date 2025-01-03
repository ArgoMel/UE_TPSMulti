// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuffComponent.generated.h"

class ABaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSMULTI_API UBuffComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	friend class ABaseCharacter;
	UBuffComponent();
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TObjectPtr<ABaseCharacter> Character;

	//Heal buff
	bool bHealing = false;
	float HealingRate = 0.f;
	float AmountToHeal = 0.f;

	// Shield buff
	bool bReplenishingShield = false;
	float ShieldReplenishRate = 0.f;
	float ShieldReplenishAmount = 0.f;

	//Speed buff
	FTimerHandle SpeedBuffTimer;
	float InitialBaseSpeed;
	float InitialCrouchSpeed;

	// Jump buff
	FTimerHandle JumpBuffTimer;
	float InitialJumpVelocity;

private:
	void ResetSpeeds();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpeedBuff(float BaseSpeed, float CrouchSpeed);
	void MulticastSpeedBuff_Implementation(float BaseSpeed, float CrouchSpeed);

	void ResetJump();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastJumpBuff(float JumpVelocity);
	void MulticastJumpBuff_Implementation(float JumpVelocity);

protected:
	void HealRampUp(float DeltaTime);
	void ShieldRampUp(float DeltaTime);

public:
	void Heal(float HealAmount, float HealingTime);
	void ReplenishShield(float ShieldAmount, float ReplenishTime);
	void BuffSpeed(float BuffBaseSpeed, float BuffCrouchSpeed, float BuffTime);
	void BuffJump(float BuffJumpVelocity, float BuffTime);
	void SetInitialSpeeds(float BaseSpeed, float CrouchSpeed);
	void SetInitialJumpVelocity(float Velocity);
};
