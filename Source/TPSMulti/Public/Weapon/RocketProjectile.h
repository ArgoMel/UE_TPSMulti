// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Weapon/Projectile.h"
#include "RocketProjectile.generated.h"

class URocketMovementComponent;

UCLASS()
class TPSMULTI_API ARocketProjectile : public AProjectile
{
	GENERATED_BODY()
public:
	ARocketProjectile();
protected:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& Event) override;
#endif
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

protected:
	UPROPERTY(EditAnywhere)
	USoundCue* ProjectileLoop;

	UPROPERTY()
	UAudioComponent* ProjectileLoopComponent;

	UPROPERTY(EditAnywhere)
	USoundAttenuation* LoopingSoundAttenuation;

	UPROPERTY(VisibleAnywhere)
	URocketMovementComponent* RocketMovementComponent;
};
