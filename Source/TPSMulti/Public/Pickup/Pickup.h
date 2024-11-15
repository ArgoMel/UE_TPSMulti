// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <TPSMulti/TPSMulti.h>
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

class USphereComponent;
class USoundCue;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class TPSMULTI_API APickup : public AActor
{
	GENERATED_BODY()	
public:	
	APickup();
protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	USphereComponent* OverlapSphere;

	UPROPERTY(EditAnywhere)
	USoundCue* PickupSound;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* PickupEffectComponent;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* PickupEffect;

	FTimerHandle BindOverlapTimer;
	float BindOverlapTime = 0.25f;

protected:
	UPROPERTY(EditAnywhere)
	float BaseTurnRate = 45.f;

private:
	void BindOverlapTimerFinished();

protected:
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
