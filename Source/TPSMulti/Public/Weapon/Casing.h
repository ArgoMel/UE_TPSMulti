// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Casing.generated.h"

class USoundCue;

UCLASS()
class TPSMULTI_API ACasing : public AActor
{
	GENERATED_BODY()
public:
	ACasing();
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CasingMesh;

	UPROPERTY(EditAnywhere)
	float ShellEjectionImpulse;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundCue> ShellSound;

private:
	UFUNCTION()
	void Delay_Destroy();

protected:
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
