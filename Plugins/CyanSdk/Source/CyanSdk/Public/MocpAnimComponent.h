// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MocpAnimApplyUtilities.h"
#include "MocpAnimComponent.generated.h"


UCLASS()
class CYANSDK_API UMocpAnimComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMocpAnimComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void WriteMocapData(FName BoneName, FTransform BoneTransform);
	void InitWithSkeleton(USkeleton* Skeleton);
public:
	UPROPERTY()
	FMocpPoseData PoseData;
		
};
