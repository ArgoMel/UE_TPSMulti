// Fill out your copyright notice in the Description page of Project Settings.

#include "CaptureTheFlag/FlagZone.h"

AFlagZone::AFlagZone()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFlagZone::BeginPlay()
{
	Super::BeginPlay();
}

void AFlagZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

