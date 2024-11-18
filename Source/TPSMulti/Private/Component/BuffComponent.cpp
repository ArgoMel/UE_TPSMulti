// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/BuffComponent.h"
#include "Character/BaseCharacter.h"

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
}

void UBuffComponent::MulticastSpeedBuff_Implementation(float BaseSpeed, float CrouchSpeed)
{
}

void UBuffComponent::ResetJump()
{
}

void UBuffComponent::MulticastJumpBuff_Implementation(float JumpVelocity)
{
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
}

void UBuffComponent::Heal(float HealAmount, float HealingTime)
{
	bHealing = true;
	HealingRate = HealAmount /HealingTime;
	AmountToHeal += HealAmount;
}

void UBuffComponent::ReplenishShield(float ShieldAmount, float ReplenishTime)
{
}

void UBuffComponent::BuffSpeed(float BuffBaseSpeed, float BuffCrouchSpeed, float BuffTime)
{
}

void UBuffComponent::BuffJump(float BuffJumpVelocity, float BuffTime)
{
}

void UBuffComponent::SetInitialSpeeds(float BaseSpeed, float CrouchSpeed)
{
}

void UBuffComponent::SetInitialJumpVelocity(float Velocity)
{
}
