// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup/PickupSpawnPoint.h"
#include "Pickup/Pickup.h"

APickupSpawnPoint::APickupSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void APickupSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	StartSpawnPickupTimer(nullptr);
}

void APickupSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickupSpawnPoint::SpawnPickup()
{
}

void APickupSpawnPoint::SpawnPickupTimerFinished()
{
}

void APickupSpawnPoint::StartSpawnPickupTimer(AActor* DestroyedActor)
{
}

