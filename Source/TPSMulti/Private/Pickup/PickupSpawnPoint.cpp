// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup/PickupSpawnPoint.h"
#include "Pickup/Pickup.h"

APickupSpawnPoint::APickupSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	SpawnPickupTimeMin = 1.f;
	SpawnPickupTimeMax = 5.f;
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
	int numPickupClasses = PickupClasses.Num();
	if(numPickupClasses>0)
	{
		int32 selection = FMath::RandRange(0,numPickupClasses-1);
		SpawnedPickup=GetWorld()->SpawnActor<APickup>(PickupClasses[selection],GetActorTransform());
		if(SpawnedPickup&&HasAuthority())
		{
			SpawnedPickup->OnDestroyed.AddDynamic(this, &ThisClass::StartSpawnPickupTimer);
		}	
	}
}

void APickupSpawnPoint::SpawnPickupTimerFinished()
{
	if(HasAuthority())
	{
		SpawnPickup();
	}
}

void APickupSpawnPoint::StartSpawnPickupTimer(AActor* DestroyedActor)
{
	const float spawnTime = FMath::RandRange(SpawnPickupTimeMin,SpawnPickupTimeMax);
	GetWorld()->GetTimerManager().SetTimer(SpawnPickupTimer,this,&ThisClass::SpawnPickupTimerFinished, spawnTime);
}

