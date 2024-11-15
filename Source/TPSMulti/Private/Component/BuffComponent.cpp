// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/BuffComponent.h"

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
}

void UBuffComponent::ShieldRampUp(float DeltaTime)
{
}

void UBuffComponent::Heal(float HealAmount, float HealingTime)
{
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
