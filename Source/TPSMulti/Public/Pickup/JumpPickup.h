// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Pickup/Pickup.h"
#include "JumpPickup.generated.h"

UCLASS()
class TPSMULTI_API AJumpPickup : public APickup
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	float JumpZVelocityBuff = 3000.f;

	UPROPERTY(EditAnywhere)
	float JumpBuffTime = 30.f;

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
