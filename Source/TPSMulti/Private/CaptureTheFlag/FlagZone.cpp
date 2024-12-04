// Fill out your copyright notice in the Description page of Project Settings.

#include "CaptureTheFlag/FlagZone.h"

#include "Components/SphereComponent.h"
#include "GameMode/CaptureTheFlagGameMode.h"
#include "Weapon/FlagWeapon.h"

AFlagZone::AFlagZone()
{
	PrimaryActorTick.bCanEverTick = false;

	ZoneSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ZoneSphere"));
	SetRootComponent(ZoneSphere);
}

void AFlagZone::BeginPlay()
{
	Super::BeginPlay();
	ZoneSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);
}

void AFlagZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFlagZone::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFlagWeapon* overlappingFlag = Cast<AFlagWeapon>(OtherActor);
	if (overlappingFlag && overlappingFlag->GetTeam() != Team)
	{
		const ACaptureTheFlagGameMode* gameMode = GetWorld()->GetAuthGameMode<ACaptureTheFlagGameMode>();
		if (gameMode)
		{
			gameMode->FlagCaptured(overlappingFlag, this);
		}
		overlappingFlag->ResetFlag();
	}
}

