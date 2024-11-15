// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Pickup/Pickup.h"
#include "AmmoPickup.generated.h"

UCLASS()
class TPSMULTI_API AAmmoPickup : public APickup
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	int32 AmmoAmount = 30;

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
