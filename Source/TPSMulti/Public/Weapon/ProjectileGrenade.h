// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Weapon/Projectile.h"
#include "ProjectileGrenade.generated.h"

UCLASS()
class TPSMULTI_API AProjectileGrenade : public AProjectile
{
	GENERATED_BODY()
public:
	AProjectileGrenade();
protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

private:
	UPROPERTY(EditAnywhere)
	USoundCue* BounceSound;

protected:
	UFUNCTION()
	void OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);
};
