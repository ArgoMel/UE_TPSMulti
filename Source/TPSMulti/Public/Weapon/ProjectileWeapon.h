// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Weapon/Weapon.h"
#include "ProjectileWeapon.generated.h"

class AProjectile;

UCLASS()
class TPSMULTI_API AProjectileWeapon : public AWeapon
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ServerSideRewindProjectileClass;

public:
	virtual void Fire(const FVector& HitTarget) override;
};
