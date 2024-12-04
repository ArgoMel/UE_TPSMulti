// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MocapBaseApi.h"
#include "GameFramework/Character.h"
#include "MocapLiveHand.h"
#include "MocapHandler.h"
#include "MocapBodyApi.generated.h"


UCLASS()
class CYANSDK_API AMocapBodyApi : public ACharacter
{
	GENERATED_BODY()
public:
	AMocapBodyApi();
public:
	void  setPelvisOringal(FVector op);
	FTransform getTipFt(int tipid);
	//create span  point
	float  getavaLength(int id);
	void  initLength(TArray<FVector> Fjoints, void* vComponent);

	TArray<FVector>  creatSpan(TArray<FVector> Fjoints, void* vComponent);
	FVector spanDistrub(int self, TArray<FVector>  Fjoints);
	TArray<float>    GetAvatarHandIkLength();
	// foot holding ,create new joint targets
	TArray<FVector>  pelvistFootHold(TArray<FVector> Fjoints, void* vComponent);
	FVector anklefootShift(TArray<FVector> Fjoints, FVector  vfoot, bool bright);
	TArray<FVector>  otherReset(TArray<FVector> Fjoints, void* vComponent);
	// int root diff  between avatar and joint 
	void initRootDiff(TArray<FVector> Fjoints, void* vComponent);
	FName getBodyName(int id);
	FName getHandName(int id);
	TArray<FName>  getHandName();
	float  getPenvisFloorMin();
public:
	FVector    WordLocationUe(FName boneName);
	FVector    LocalLocationUe(FName boneName);
	FVector    WordLocation(FName boneName);
	FVector    _UE2(FVector source);
	FVector    _2UE(FVector source);

	FRotator   WordRotator(FName boneName);
	FRotator   WordRotatorUe(FName boneName);
	FRotator   LocalRotatorUe(FName boneName);
	FQuat       WordQuaUe(FName boneName);
	FQuat       LocalQuaUe(FName boneName);
	void saveModelInf(int index);
	bool  loadModelInf(int switch_on);
	void loadModelPlaychange(int index);
	void  setModelStatus(int mid);
	bool  loadModelFlag();
	bool  isPoseInited();
	float   getPelvisdis();


	void UpdateValuesetting(FJS_Modifycharacterparaneters Realvariable);

	void    floorDis(float  floor, float  footdis, float kneelwide, float initialization, float tyny, float elblowrorate, float sholder, float infaceuprotate);
	void       setAnkleType(int tid);
	FVector  getPointTarget(int from, int to, FVector vjoint, bool bChiefhand, bool bhand, TArray<FVector> Fjoints);// get avatar new child   joint target 
	FVector  getJointVec(int from, int to0, int to1, TArray<FVector> Fjoints, bool bChiefhand, bool bhand);
	//avart new taget by fixed length
	FVector   getPointTarget(int from, int to0, int to1, FVector vjoint, bool bChiefhand, bool bhand, TArray<FVector> Fjoints);
	FVector   getNomoveTarget(int from, FVector rc);
	FVector  getAvatartNewVec(int from, int to0, int to1, bool bChiefhand, bool bhand, TArray<FVector> Fjoints);
	FVector  getAvatartVec(int from, int to0, int to1, bool bChiefhand, bool bhand, TArray<FVector> Fjoints);
	void      HipRorate(void* vComponent, bool bright, TArray<FVector> Fjoints, FQuat curqhip);

	FVector    getJointVec(int from, int to, TArray<FVector> Fjoints, bool bChiefhand, bool bhand);
	FVector    getAvatartNewVec(int from, int to, bool bChiefhand, bool bhand, TArray<FVector> Fjoints);
	FVector    getAvatartVec(int from, int to, bool bChiefhand, bool bhand, TArray<FVector> Fjoints);
	FQuat      changeQuat(FVector v1, FVector v2, bool noChangeZ, float ratio);
	FQuat       caculateRot2(int self, int from, int to0, int to1, void* vComponent, bool bChiefhand, bool bhand, TArray<FVector> Fjoints, FQuat selfRot);
	FQuat       caculateRot(int self, int from, int to, void* vComponent, bool bSelfhand, bool bhand, TArray<FVector> Fjoints, FQuat selfRot); \
		FQuat        caculateFinger(int self, void* vComponent, bool bright, TArray<FVector> Fjoints, FQuat qwrist, bool bupdate);

	FTransform    getFingerTF(int self, TArray<FVector> Fjoints, bool bright, bool bChild);
	void          fingerRorate(void* vComponent, bool bright, TArray<FVector> Fjoints, FQuat curqwrist);
	FQuat       caculateSet(int self, TArray<FVector> Fjoints, void* vComponent, FQuat qset);
	FQuat       caculateHandSet(int self, TArray<FVector> Fjoints, void* vComponent, FQuat qset);
	FQuat       SetPos(int self, void* vComponent, TArray<FVector> Fjoints, FVector pos);
	void       caculatePos(int self, int from, int to, int otherid, void* vComponent, bool bhand, TArray<FVector> Fjoints);
	FQuat     caculateOther(bool elblowBywrist, int self, void* vComponent, bool bselfhand, TArray<FVector> Fjoints, float ratio, FQuat qparent);
	FQuat      caculateOtheElblow(int self, void* vComponent, bool bselfhand, TArray<FVector> Fjoints, float ratio, FQuat qparent);

	FQuat      caculateBywrist(int self, void* vComponent, bool bselfhand, TArray<FVector> Fjoints, FQuat qother, float ratio);
	FQuat      caculateOtheWrist(bool elblowBywrist, int self, void* vComponent, bool bselfhand, TArray<FVector> Fjoints, float ratio, FQuat qparent);
	FQuat      caculateInitBack(int self, void* vComponent, TArray<FVector> Fjoints);
	FQuat      caculateInitSide(int self, void* vComponent, TArray<FVector> Fjoints);
	int        defineWristDirect(FVector v1, FVector v2);
	int        defineAnkleDirect(FVector v1, FVector v2);
	FVector    getInitFingerPose(FVector v1, FVector v2, TArray<FVector> Fjoints, int fingersID, bool bright, bool belblow);
	FQuat     getInitFingerQua(FVector v1, FVector v2, TArray<FVector> Fjoints, bool bright, bool belblow);
	FVector    getInitKneelPose(FVector v1, FVector v2, int legid, bool bright, TArray<FVector> Fjoints);
	bool       bodyfloorCheck(int self, TArray<FVector> Fjoints);
	bool       bodyfloorCheckUe(int self, TArray<FVector> Fjoints);
	TArray<FVector>  floorHold(TArray<FVector> Fjoints, void* vComponent);
	FQuat    getInitKneelQua(FVector v1, FVector v2, int legid, bool bright, TArray<FVector> Fjoints);
	FQuat    caculateComp(int self, FQuat qother, FQuat qself, void* vComponent, bool bselfhand, bool bhand, TArray<FVector> Fjoints, bool bUpdate);
	FQuat    caculateNeckNormal(int self, TArray<FVector> Fjoints, void* vComponent, bool bUpdate);
	FQuat    caculateHeadNormal(int self, TArray<FVector> Fjoints, void* vComponent, bool bUpdate);


	FQuat    WristSelf(int id, TArray<FVector> Fjoints, bool isupdate, float ratio);
	FQuat    ElblowSelf(int id, TArray<FVector> Fjoints);
	FQuat    KneelSelf(int id, TArray<FVector> Fjoints);
	FQuat    AnkleSelf(int id, TArray<FVector> Fjoints);
	void     CaculateLegMid(int id, TArray<FVector> Fjoints, void* vComponent, FQuat qkneel);
	void     CaculateArmMid(int id, TArray<FVector> Fjoints, void* vComponent, FQuat qelblow);
	FQuat    ShldSelf(int id, void* vComponent, TArray<FVector> Fjoints);
	void      limitUpdate(TArray<FVector> Fjoints, void* vComponent);
	FQuat      smooth(int self, int to, TArray<FVector> Fjoints, FQuat qself, FTransform ft);
	TArray<FVector>    getPoseJoints();
	TArray<FVector2D>  getMap();
	TArray<FVector>    getHandJoints();
	TArray<FVector2D>  getHandMap();
	TArray<FVector>    above(TArray<FVector> Fjoints);
	void SetFingers2Init(bool rhand, FQuat qwrist, void* vComponent, USkeletalMeshComponent* pSkeletalMeshComp);
	void avatarAbove(int  id, TArray<FVector> Fjoints, void* vComponent);
	void  setPlameSmooth(bool  isPlame, bool bleft);
	void SetIKnameInit(int mtype, int mid, TArray<FName> FIk, TArray<FName> FIkAdd, FName  FIkroot, TArray<FName> FHandIk);
	void Init(void* vComponent, void* SkeletalMesh);
	int  getPid(int childid);
	int  getHandPid(int childid);
	int getCid(int parentid);
	FQuat  Clean180(FQuat self);
	bool  isDated();
	bool  isFixed();
	bool  isModelInited();
	void ResetRootDiff();
	FVector  gotRootDiff();
	FQuat QSlerp(FQuat Current, FQuat Target, float alpha);
	FQuat    caculateSelf(int selffrom, int selfto, int from, int to, void* vComponent, bool bSelfhand, bool bhand, TArray<FVector> Fjoints, float ratio);
	FQuat    caculateWrist(bool elblowBywrist, int id, TArray<FVector> Fjoints, void* vComponent, FQuat qother, bool update, float ratio);
	FVector  getLeftwrist(bool bLeft);
	bool     isSpanPoints(int self, TArray<FVector> Fjoints);
	//tgside  for span inited
	FQuat    getspanQua(int self, TArray<FVector> Fjoints);
	FVector  getspanPos(int self, TArray<FVector> Fjoints);
	void  defineDirect(TArray<FVector> Fjoints);
	TArray<FVector>  resetException(TArray<FVector> Fjoints);
	FQuat  caculateAnkle(int self, void* vComponent, TArray<FVector> Fjoints);
	float  gettestinfo2();
	float gettestinfo();//
	void settestinfo(float  v1);//
	void  setFingers();
	void InitBodyDate();
	TArray<FVector>  legratio(TArray<FVector>  vbodyrest);
	//ik name 
	//length
	//get name
	void setIK(int  id, TArray<FVector> Fjoints, void* vComponent, bool bhand, int mtype, int modelid, bool isRGB);
	void InitPinkyTF();
	TArray<FVector> resethans(TArray<FVector> fjoints);
	//AMocapBaseApi* BaseAPI = nullptr;
	//AMocapLiveHand* handapi = nullptr;
	FQuat  caculateElblowByWrist(int id, TArray<FVector> Fjoints, void* vComponent, bool bhand, FQuat self);

	bool bHandoccface(TArray<FVector> fjoints);
	bool bMovefast(TArray<FVector> fjoints);
	bool bAngleshift(TArray<FVector> fjoints);
	static bool getTyny();
private:
	void InitStandardBoneLength();
	float CalculateBoneWeightVal(float StranardBoneVal, float StranardBoneSize, float CurBoneSize);
	FTransform GetMathFingerTF(bool bLocal, const FName& InName);
	void SetFingerCount();
};
