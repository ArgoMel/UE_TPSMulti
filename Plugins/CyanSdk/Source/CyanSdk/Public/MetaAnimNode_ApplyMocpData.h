// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"MocpAnimApplyUtilities.h"
#include "Animation/AnimNodeBase.h"
#include "MetaAnimNode_ApplyMocpData.generated.h"

class UMocpAnimComponent;
PRAGMA_DISABLE_DEPRECATION_WARNINGS

USTRUCT(BlueprintInternalUseOnly)
struct CYANSDK_API FMetaAnimNode_ApplyMocpData : public FAnimNode_Base
{
	GENERATED_BODY()
public:
	FMetaAnimNode_ApplyMocpData();
	virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext& Context) override;
	virtual void Update_AnyThread(const FAnimationUpdateContext& Context) override;
	virtual void Evaluate_AnyThread(FPoseContext& Output) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		FPoseLink InputPose;
private:
	UPROPERTY()
		TWeakObjectPtr<UMocpAnimComponent> MocapComponent;

	UPROPERTY()
		FMocpPoseData PoseCache;

};
