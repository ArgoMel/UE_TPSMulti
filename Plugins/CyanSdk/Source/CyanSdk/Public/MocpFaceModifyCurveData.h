// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/AnimNodeBase.h"
#include "MocpStructure.h"
#include "MocpFaceModifyCurveData.generated.h"

/**
 *
 */

PRAGMA_DISABLE_DEPRECATION_WARNINGS


USTRUCT(BlueprintInternalUseOnly)
struct CYANSDK_API FMocpFaceModifyCurveData :public FAnimNode_Base
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Source)
    FPoseLink SourcePose;
    UPROPERTY(EditAnywhere, Category = Settings, meta = (PinShownByDefault))
    bool bRequiresInitializationBody=true;
    FMocpFaceModifyCurveData();
    void Initcurvename();
    // FAnimNode_Base interface
    virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
    virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext& Context) override;
    virtual void Evaluate_AnyThread(FPoseContext& Output) override;
    virtual void Update_AnyThread(const FAnimationUpdateContext& Context) override;

    void SetFaceValues(const FFaceChangeValue& FaceValueObject);


    //How to change the weight of a curve
    void ResetFaceCurveWeight(FPoseContext& Output);
    //Provides variable interfaces to people classes
    static FFaceChangeValue& GotFaceWeightST();
    static TArray<FName> GotArtkitsCurveName();
    static void SetFaceWeightST(FFaceChangeValue& FaceWeightst);




private:
  
    float CachedDeltaTime;




};