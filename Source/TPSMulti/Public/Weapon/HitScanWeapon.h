// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Weapon/Weapon.h"
#include "HitScanWeapon.generated.h"

class UParticleSystem;

UCLASS()
class TPSMULTI_API AHitScanWeapon : public AWeapon
{
	GENERATED_BODY()
protected:
	virtual void Fire(const FVector& HitTarget) override;

private:
	UPROPERTY(EditAnywhere)
	UParticleSystem* BeamParticles;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundCue* FireSound;

protected:
	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere)
	USoundCue* HitSound;

protected:
	void WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit);
};
