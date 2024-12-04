// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MocapBaseApi.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "MocapLiveHand.generated.h"


UCLASS()
class CYANSDK_API AMocapLiveHand : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMocapLiveHand();
	//FQuat fingersLimited(int id, FQuat ft, FVector fvparent, TArray < FTransform> avats);
	TArray<FQuat> getdataQua(int id);
	void  savedataQua(TArray<FQuat>  qdata, int id);
	TArray<FVector> getdataFv(int id);
	void savedataFv(TArray<FVector>  fdata, int id);


	void setfingerBendType(int  ftype);


	void setBendFingers0_90(bool  bleft, bool  bvaue);

	void setfingerShiftType(int  ftype);
	void setShiftFingers0_90(bool  bleft, bool  bvaue);

	TArray<FVector> rePosition(TArray<FVector> fjoints);
	TArray<FVector>  resetHands(TArray<FVector> fjoints ,bool isrgb);
	TArray<FVector>  smoothHands(TArray<FVector> fjoints);
	TArray<FVector> getLast(TArray<FVector> fjoints);

	FRotator getWorldFRotator(FTransform parentft, FRotator frchild);
	FRotator getLocalFRotator(FTransform parentft, FRotator frchild);
	bool  handfastmove(bool bright, bool lblarge);
	void CreatFist(TArray<FVector> fjoints, FQuat qwrist, void* vComponent, TArray<FName>   FHandIkNames, bool rhand, USkeletalMeshComponent* pSkeletalMeshComp);


	TArray<FVector> CheckFist(TArray<FVector> fjoints, bool rhand);
	bool  CheckPalm(TArray<FVector> fjoints, bool rhand, float minifloor);
	TArray<FVector>  CreatePalm(TArray<FVector> fjoints, float minifloor, TArray<float>  AvatarHandIkLength);
	int  toCarpal(int proximalid);
	void FingersLimitation(TArray<FVector> fjoints, TArray<FTransform> avatslocal, int id, FQuat qself, FQuat qCarpal, void* vComponent, TArray<FName>   FHandIkNames, bool rhand, USkeletalMeshComponent* pSkeletalMeshComp);
	void ClivicLimitation(TArray<FVector> fjoints, int id, FQuat qwrist, void* vComponent, TArray<FName>   FHandIkNames, bool rhand, USkeletalMeshComponent* pSkeletalMeshComp, TArray < FTransform> avats);
	void MissingKeep(TArray<FVector> fjoints, int id, FQuat qwrist, void* vComponent, TArray<FName>   FHandIkNames, bool rhand, USkeletalMeshComponent* pSkeletalMeshComp);
	FQuat saveOrGetlast(int id, FQuat qdata, bool isSave);
	void  saveLocal(int id, FQuat qlocal);
	FQuat getLocal(int id);
	FQuat finglesLocal(int id, FQuat qlocal, bool bright, bool bSave);
	void  SetCureentQua(int id, FQuat qwrist, void* vComponent, TArray<FName>   FIkNames, USkeletalMeshComponent* pSkeletalMeshComp);
	void onlyBend(TArray<FVector> fjoints, USkeletalMeshComponent* body_pSkeletalMeshComp, TArray<FTransform> fingers, FQuat qwrist, void* vComponent, TArray<FName>   FHandIkNames, bool rhand, USkeletalMeshComponent* pSkeletalMeshComp);
	void setSmoothlyQua(FQuat qtemp, FQuat qwrist, int fingid);
	FQuat getSmoothlyQua(int fingid);

	void setSmoothlyQuaLast(FQuat qtemp, int fingid);
	FQuat getSmoothlyQuaLast(int fingid);
	FQuat getThumbfistMini(bool bright, int fid);
    void setThumbfistMini(bool bright, FQuat qtemp1, FQuat qtemp2, FQuat qtemp3, TArray<FVector> fjoints);
	
	void setWristSmoothlyQua(FQuat qtemp, int wristid);
	FQuat getWristSmoothlyQua(int wristid);
	FQuat BendCaculate(int fid, TArray<FTransform> fingers, bool rhand, TArray<FVector> fjoints);
	FRotator ShiftCaculate(int fid, TArray<FTransform> fingers, bool rhand, TArray<FVector> fjoints);
	FQuat cQuat(FVector v1, FVector v2, bool noChangeZ, float ratio);
	TArray<FVector> resethandlen(TArray<FVector> fjoints, TArray<float>  body_AvatarHandIkLength);
	FQuat QFingerSlerp(FQuat Current, FQuat Target, float alpha);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
