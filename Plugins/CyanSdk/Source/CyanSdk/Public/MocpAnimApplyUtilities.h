// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/Skeleton.h"
#include "Components/SkeletalMeshComponent.h"
#include "MocpAnimApplyUtilities.generated.h"

USTRUCT()
struct CYANSDK_API FMocpPoseData
{
	GENERATED_BODY()

	UPROPERTY()
	TEnumAsByte<enum EBoneControlSpace> PoseType = EBoneControlSpace::BCS_BoneSpace;
private:
	UPROPERTY()
	TArray<FName> BoneNames;
	UPROPERTY()
	TArray<FTransform> BonePoses;
public:
	//clear all name buffer and pose buffer
	void Clear();
	//Init pose data by skeleton. Will build Name buffer for all bones and fill pose buffer by skeleton reference pose
	void InitWithSkeleton(USkeleton* inSkeleton);
	//Set a pose transform for bone. If name didn't exist, will add it into buffer 
	void SetBoneTransform(FName BoneName, const FTransform& InBoneTF);
	//get bone transform from buffer
	bool GetBoneTransform(FName BoneName, FTransform& BoneTFOut);
	//reset all buffer. It will override all exist bone name and data.
	void ResetAllBuffer(const TArray<FName>& inBoneName, const TArray<FTransform>& inBoneTF);
	//set data space
	void SetDataSpace(EBoneControlSpace Space);
	FTransform GetBoneTransform(FName BoneName);
	const TArray<FName>& GetBoneNameArr() { return BoneNames; }
	const TArray<FTransform>& GetTransformArr() { return BonePoses; }
	void GetBonesScale(const USkeletalMeshComponent* inSkelMeshComp);
public:
	void InitIkNames(TArray<FName>* handIkNames, TArray<FName>* avatarIkName, TArray<FName>* avatarAddIkName, const FName& inanimAvatarRootIkName);
	TArray<FName> GetHandIKNames();
	TArray<FName> GetAvatarIkNames();
	TArray<FName> GetAvatarAddIkNames();
	FName GetAvatarIkRoot();
};