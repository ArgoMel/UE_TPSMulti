// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlagZone.generated.h"

UCLASS()
class TPSMULTI_API AFlagZone : public AActor
{
	GENERATED_BODY()
public:
	AFlagZone();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
