// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "FlagWeapon.generated.h"

UCLASS()
class TPSMULTI_API AFlagWeapon : public AWeapon
{
	GENERATED_BODY()
public:
	AFlagWeapon();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnEquipped() override;
	virtual void OnDropped() override;
public:
	virtual void Dropped() override;

private:
	FTransform InitialTransform;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DynamicMaterialInstances;
	
public:
	void ResetFlag();
	
	FORCEINLINE FTransform GetInitialTransform() const { return InitialTransform; }
};
