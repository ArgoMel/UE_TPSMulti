// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MocapBaseApi.h"
#include "MocapLiveFace.h"
#include "MocapLiveHand.h"
#include "MocapRecord.h"
#include "GameFramework/Character.h"
#include "Runtime/Sockets/Public/SocketSubsystem.h"
#include "MocapHandler.generated.h"


UCLASS()
class CYANSDK_API AMocapHandler : public ACharacter
{
	//save 10 frames for cache
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMocapHandler();

	int Getplugeversion();


public:
	// Called when the game starts or when spawned
	void start(UObject* characterObj);
	void startOpRecod();
	void stopOpRecod();
	void play();
	void stop();
	void restart();
	void go();
	void back();
	void go20();
	void back20();
	bool getStatus(int id);
	static int getiStatus();
	int getHandsStatus(int id);

public:
	TArray<FVector>  gotData();
	bool           checkData();
	TArray<FVector>  _Exception(int id, TArray<FVector> FjointsTemp);
	TArray<FVector>  _Exceptionhand(int status, bool bright, TArray<FVector> FjointsTemp);
	bool getisDone();
	bool IS_RGB();
	bool GetFist(bool bright);
	void SetisDone(bool isset);
	TArray<FVector> Gotfacerecdata();
	bool hasData();
	void cancelData();

public:
	void  oneHandJointFrame(int id);
	void  oneJointFrame();
public:
	void* pbody;
public:
	TArray<FVector> Fjoints;
	TArray<short>   FCoin;
	TArray<FVector>   FLhand;
	TArray<FVector>   FRhand;



public:
	AMocapBaseApi* BaseAPI = nullptr;
	void  setWritedata(bool bvalue);
	bool  getWritedata();
	/*AMocapRecord* recOp = nullptr;*/
	/*AMocapLiveFace * face = nullptr;*/
	USkeletalMeshComponent* TestSkeletal_Mesh;
	/*AMocapLiveHand* handapi = nullptr;*/
	int getFPS(int rfps, bool isDated);
	float getSleep();
	float getTest();
	void SetRec(bool isset);
	bool isRec();
	static void freeSomething();
	void changeSleepVal(bool isAdd);
	void Playbegin();
	void changeReset();
	static bool GetoutCamera();
	static void SetIPStr(FString Ipadd);
	float  gettestinfos();
	int getHandStatus(bool isRight, int type);

};

