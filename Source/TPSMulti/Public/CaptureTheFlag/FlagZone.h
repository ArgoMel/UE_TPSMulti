// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSMulti/TPSMulti.h"
#include "GameFramework/Actor.h"
#include "FlagZone.generated.h"

class USphereComponent;

UCLASS()
class TPSMULTI_API AFlagZone : public AActor
{
	GENERATED_BODY()
public:
	AFlagZone();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	USphereComponent* ZoneSphere;
	
public:	
	UPROPERTY(EditAnywhere)
	ETeam Team;
	
protected:
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
