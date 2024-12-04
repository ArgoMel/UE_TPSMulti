// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include<iostream>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MocapRecord.generated.h"

UCLASS()
class CYANSDK_API AMocapRecord : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AMocapRecord();
public:
	int totalCnt;//totals  filled frames
	short rStatus;    //0: normol,do nothing   1:is  recording and   no more than maxFrame   2: record full; 
	bool bStop = false;
	TArray<FVector> dataFaces;//saved face data
public:
	void pSwitch();
	void  gotofrm(int fid);
	void pSwitchOnceStart();
	void deleteFolder();
	void pSwitchOnceStop();
	void  writeRdata(TArray<FVector> vdata, int type);  // fill data to local
	void ForceInit();
	static int  outGetCurID();
	static int  GetCurWriteFrame();
	static void	JumpToZeroFrame();
	static void PrintTraceStack(int Depth);
	void getFrame(int type, TArray<FVector>& recBody);//0:body 1:face , as api to fetch  one fram  data
	FString outGetinfo();
	UFUNCTION(BlueprintCallable, Category = " MocpRecord")
		void  next();    //play next frame
	UFUNCTION(BlueprintCallable, Category = " MocpRecord")
		void  last();     //play last frame
	UFUNCTION(BlueprintCallable, Category = " MocpRecord")
		void  last20();   //play last 20 frame
	UFUNCTION(BlueprintCallable, Category = " MocpRecord")
		void  next20();  //play next 20 frame
	UFUNCTION(BlueprintCallable, Category = " MocpRecord")
		void  stop();     //stop playing
	UFUNCTION(BlueprintCallable, Category = " MocpRecord")
		void  start();    //start playing 
	UFUNCTION(BlueprintCallable, Category = " MocpRecord")
		void  restart();  //start playing from 0 frame
	TArray<FVector> GetVecArrByFrame(int indexFrame);
	void WriteDataInText(TArray<FVector> VecData,int curFrame);
	void ReSetData();
	FString DeleteFrame(int frame);
	void FastFowardFrame();
	void FastBackwardFrame();
	FString RecoverAllFile();
protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private://record date operation
	void  fetchRData(int type); //got all data from files
	FString getFileFolderPath();

	TArray<FString> Tokennize(TArray<FString> Split_string, FString symbol);
	bool isFrameCanMove(int curFrame, int spanNum, bool isNext);

	int  getDirFileNum(const std::string& inPath);

	bool CheckFrameIsBackUp(int frameIndex);
	bool BackUpFrameFile(int frameIndex);
	int CheckFindType(int frameIndex);
	void SmoothFrameByFindType(int frameIndex, int findType);
	void FindNotSmoothFrameIndex(int &preFrame,int &nextFrame,int curFrame);
	void SmoothFramesByIndex(int frameIndex, int leftIndex, int rightIndex);
	void CheckCurPlayid();
	void GetAllBackupFileName(TArray<FString> &strNames);
	void RecoverFile(TArray<FString>&strNames);
	int GetAllFilNum();

};

