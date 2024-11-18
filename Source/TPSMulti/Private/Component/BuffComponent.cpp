// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/BuffComponent.h"
#include "Character/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UBuffComponent::UBuffComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	InitialBaseSpeed = 0.f;
	InitialCrouchSpeed = 0.f;
	InitialJumpVelocity = 0.f;
}

void UBuffComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	HealRampUp(DeltaTime);
	ShieldRampUp(DeltaTime);
}

void UBuffComponent::ResetSpeeds()
{
	if (!Character ||
		!Character->GetCharacterMovement())
	{
		return;
	}
	Character->GetCharacterMovement()->MaxWalkSpeed = InitialBaseSpeed;
	Character->GetCharacterMovement()->MaxWalkSpeedCrouched = InitialCrouchSpeed;
	MulticastSpeedBuff(InitialBaseSpeed, InitialCrouchSpeed);
}

void UBuffComponent::MulticastSpeedBuff_Implementation(float BaseSpeed, float CrouchSpeed)
{
	if (!Character ||
		!Character->GetCharacterMovement())
	{
		return;
	}
	Character->GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
	Character->GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
}

void UBuffComponent::ResetJump()
{
	if (!Character ||
		!Character->GetCharacterMovement())
	{
		return;
	}
	Character->GetCharacterMovement()->JumpZVelocity = InitialJumpVelocity;
	MulticastJumpBuff(InitialJumpVelocity);
}

void UBuffComponent::MulticastJumpBuff_Implementation(float JumpVelocity)
{
	if (!Character ||
		!Character->GetCharacterMovement())
	{
		return;
	}
	Character->GetCharacterMovement()->JumpZVelocity = JumpVelocity;
}

void UBuffComponent::HealRampUp(float DeltaTime)
{
	if(!bHealing||
		!Character||
		Character->IsElimmed())
	{
		return;
	}
	const float healThisFrame = HealingRate * DeltaTime;
	Character->SetHealth(FMath::Clamp(Character->GetHealth()+healThisFrame,0.f, Character->GetMaxHealth()));
	Character->UpdateHUDHealth();
	AmountToHeal -= healThisFrame;
	if(AmountToHeal<=0.f||
		Character->GetHealth()>=Character->GetMaxHealth())
	{
		bHealing = false;
		AmountToHeal = 0.f;
	}
}

void UBuffComponent::ShieldRampUp(float DeltaTime)
{
	if (!bReplenishingShield ||
		!Character ||
		Character->IsElimmed())
	{
		return;
	}
	const float replenishThisFrame = ShieldReplenishRate * DeltaTime;
	Character->SetShield(FMath::Clamp(Character->GetShield() + replenishThisFrame, 0.f, Character->GetMaxShield()));
	Character->UpdateHUDShield();
	ShieldReplenishAmount -= replenishThisFrame;
	if (ShieldReplenishAmount <= 0.f ||
		Character->GetShield() >= Character->GetMaxShield())
	{
		bReplenishingShield = false;
		ShieldReplenishAmount = 0.f;
	}
}

void UBuffComponent::Heal(float HealAmount, float HealingTime)
{
	bHealing = true;
	HealingRate = HealAmount /HealingTime;
	AmountToHeal += HealAmount;
}

void UBuffComponent::ReplenishShield(float ShieldAmount, float ReplenishTime)
{
	bReplenishingShield = true;
	ShieldReplenishRate = ShieldAmount / ReplenishTime;
	ShieldReplenishAmount += ShieldAmount;
}

void UBuffComponent::BuffSpeed(float BuffBaseSpeed, float BuffCrouchSpeed, float BuffTime)
{
	if(!Character||
		!Character->GetCharacterMovement())
	{
		return;
	}
	Character->GetWorldTimerManager().SetTimer(SpeedBuffTimer,this,&ThisClass::ResetSpeeds,BuffTime);
	Character->GetCharacterMovement()->MaxWalkSpeed = BuffBaseSpeed;
	Character->GetCharacterMovement()->MaxWalkSpeedCrouched = BuffCrouchSpeed;
	MulticastSpeedBuff(BuffBaseSpeed, BuffCrouchSpeed);
}

void UBuffComponent::BuffJump(float BuffJumpVelocity, float BuffTime)
{
	if (!Character ||
		!Character->GetCharacterMovement())
	{
		return;
	}
	Character->GetWorldTimerManager().SetTimer(JumpBuffTimer, this, &ThisClass::ResetJump, BuffTime);
	Character->GetCharacterMovement()->JumpZVelocity = BuffJumpVelocity;
	MulticastJumpBuff(BuffJumpVelocity);
}

void UBuffComponent::SetInitialSpeeds(float BaseSpeed, float CrouchSpeed)
{
	InitialBaseSpeed = BaseSpeed;
	InitialCrouchSpeed = CrouchSpeed;
}

void UBuffComponent::SetInitialJumpVelocity(float Velocity)
{
	InitialJumpVelocity = Velocity;
}
