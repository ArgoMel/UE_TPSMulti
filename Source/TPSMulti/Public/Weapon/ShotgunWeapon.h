// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Weapon/HitScanWeapon.h"
#include "ShotgunWeapon.generated.h"

UCLASS()
class TPSMULTI_API AShotgunWeapon : public AHitScanWeapon
{
	GENERATED_BODY()
public:
	AShotgunWeapon();

public:
	virtual void FireShotgun(const TArray<FVector_NetQuantize>& HitTargets);
	void ShotgunTraceEndWithScatter(const FVector& HitTarget, TArray<FVector_NetQuantize>& HitTargets);
};
