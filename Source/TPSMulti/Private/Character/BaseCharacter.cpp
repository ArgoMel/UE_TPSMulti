// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BaseCharacter.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
