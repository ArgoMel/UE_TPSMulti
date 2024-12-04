// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Animation/AnimNodeBase.h"
#include "ExtendedBodyControlAnimNode.generated.h"
/**
 * 
 */
USTRUCT()
struct FAngleRangeLimit
{
    GENERATED_BODY()

    FAngleRangeLimit()
    : LimitMin(-180, -180, -180)
    , LimitMax(+180, +180, +180)
    {}

    UPROPERTY(EditAnywhere, Category = Settings, meta = (UIMin = "-360", UIMax = "360", ClampMin = "-360", ClampMax = "360"))
    FRotator LimitMin;

    UPROPERTY(EditAnywhere, Category = Settings, meta = (UIMin = "-360", UIMax = "360", ClampMin = "-360", ClampMax = "360"))
    FRotator LimitMax;

    UPROPERTY(EditAnywhere, Category = Settings)
    FBoneReference Bone;
};

USTRUCT(BlueprintInternalUseOnly)
struct CYANSDK_API FExtendedBodyControlAnimNode :public FAnimNode_Base
{
    GENERATED_BODY()

public:
    virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
    virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext& Context) override;
    virtual void Evaluate_AnyThread(FPoseContext& Output) override;
    virtual void Update_AnyThread(const FAnimationUpdateContext& Context) override;
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
    FPoseLink InputPose;

    UPROPERTY(EditAnywhere, Category = Settings)
    TArray<FAngleRangeLimit> AngleRangeLimits;

    UPROPERTY(EditAnywhere, Category = Settings, meta = (PinShownByDefault))
        bool bPoseInitialized = false;

    UPROPERTY(EditAnywhere, Category = Settings, meta = (PinShownByDefault))
        FVector StartingPoint = FVector::ZeroVector;
protected:
    UPROPERTY()
    TMap<FName,FRotator> LastInRangeRot;
    
    bool IsOutOfBounds(const FRotator& InRotMin,const FRotator& InRot, const FRotator& InRotMaxVal);
private:
    void LimitBodyRotation(FPoseContext& Output,const FBoneContainer& BoneContainer);
    void AddExtraPosForPelvis(FPoseContext& Output, const FBoneContainer& BoneContainer);
    bool IsBoneValid(const FBoneContainer& BoneContainer, FName BoneName, int32& OutBoneIndex);
};