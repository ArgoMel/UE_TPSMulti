// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MocapBaseApi.generated.h"

UCLASS()
class CYANSDK_API AMocapBaseApi : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMocapBaseApi();
public:
	int frmcnt();
public:
	int64 GetTimestamp();
	FVector    _UE2(FVector source);
	FVector    _2UE(FVector source);
	float max(float f1, float f2);
	float min(float f1, float f2);
	//get project point in line equation
	FVector get3dlineProject(FVector p3dkeys, FVector p3d0, FVector p3d1);
	//get project point in a vetical palan  equation  p3d0 is the center point
	FVector getPlanProject(FVector p3dkeys, FVector p3d0, FVector p3d1, FVector p3d2);
	FVector getPlanProjectByNLine(FVector p3dkeys, FVector p3d0, FVector p3d1);
	FVector ex(FVector from, FVector to, float length);
	FVector ex(float ratio, FVector from, FVector to);
	FVector ex(FVector jfrom, FVector jto, FVector afrom, float alen);
	FVector ex(float ratio, FVector from, FVector to, float alen);
	FVector getPFromPlanByLnies(FVector p3dkeys, FVector pLineFrom, FVector pLineTo, FVector p3d0, FVector p3d1, FVector p3d2, float len);
	FVector getPFromPlanByPoint(FVector p3dkeys, FVector pBase, FVector p3d0, FVector p3d1, FVector p3d2, float len);
	FVector getPFromLineByPoint(FVector p3dkeys, FVector pBase, FVector p3d0, FVector p3d1, float len);
	float  getDiffFromPlanByLnies(FVector p3dkeys, FVector pLineFrom, FVector pLineTo, FVector p3d0, FVector p3d1, FVector p3d2, float len);
	float  getDiffFromLineByPoint(FVector p3dkeys, FVector pBase, FVector p3d0, FVector p3d1, float len);
	TArray<FVector>  bUpBend(FVector p3dWrist, FVector p3dLittle, FVector p3dIndex, bool righthand);
	FVector  resetExceptionBendp(FVector p3dProximal, FVector p3dMediate, FVector p3dTagget, bool bUP);
	float AngleBetweenTwoVectors(FVector VectorA, FVector VectorB);
	TArray<FVector>GetfromJsonFV(FName  modelname, FName  itemname);
	TArray<FQuat>  GetfromJsonFQ(FName  modelname, FName  itemname);
	TArray <FTransform>  GetfromJsonFTS(FName  modelname, FName  itemname);
	FTransform GetfromJsonFT(FName  modelname, FName  itemname);
	int  GetfromJsonINT(FName  modelname, FName  itemname);
	void  FillJson(FName  modelname, FName  itemname, TArray<FQuat> tdata);
	void  FillJson(FName  modelname, FName  itemname, TArray <FTransform>  tdata);
	void FillJson(FName  modelname, FName  itemname, int  tdata);
	void  FillJson(FName  modelname, FName  itemname, FTransform tdata);
	void  FillJson(FName  modelname, FName  itemname, TArray<FVector> tdata);
	FVector  getEmbed(TArray<FVector> Fjoints, int  bodyStart, int  bodyEnd, int  handStart, int handEnd);
	TArray<FVector> embedFilter(TArray<FVector> Fjoints);
	float  getDisFromline(FVector p3dkeys, FVector p3d0, FVector p3d1);
	float AngleBetweenVectors(FVector A, FVector B);
	FString GetEngineVersion();
	FString GetEXEPath();
	int Getpid();
	FVector getPlanProjectpar(FVector p3dkeys, float a, float b, float c, float d);
	FVector cyanbase(float fheellen, float fhiplen, FVector vheel, FVector vkneel, FVector vhip);

	TArray<FVector>  pelvisFootHold(TArray<FVector> Fjoints, float cbotleglen, float  ctopleglen, float chalfhiplen,int bodyid, float _Kneel_wide, bool bfloor);
	FVector getHeel(int bright, int bodyid);
	void  setHeel(TArray<FVector> Fjoints, int bodyid);
protected:
	void GetClosestPos(FVector BodyStartPos, const FVector BodyEndPos, FVector HandStartPos, FVector HandEndPos, FVector& closestBody, FVector& closestHand);
	FVector GetClosestPoint(const FVector& p1, const FVector& p2, const double& t);
	double dot(const FVector& v1, const  FVector& v2);
	FVector cross(const FVector& v1, const  FVector& v2);
	double norm(const FVector& v1);

};
