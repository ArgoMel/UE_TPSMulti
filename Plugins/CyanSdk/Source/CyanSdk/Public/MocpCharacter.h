// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MocapHandler.h"
#include "MocapBodyApi.h"
#include "MocapLiveFace.h"
#include "MocpAnimComponent.h"
#include "MocpStructure.h"
#include "CyanDataTable.h"
#include "CyanDataTable_en.h"
#include "MocapStickMen.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "InputCoreTypes.h"
#include "MocpCharacter.generated.h"

UCLASS(Config = CyanSettings, defaultconfig, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), HideCategories = ("Tags", "Activation", "Cooking", "AssetUserData", "Collision","Navigation"))
class CYANSDK_API UMocpCharacter : public UActorComponent
{
	GENERATED_BODY()
	
public:
	//Common model:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InitCyanSdk", meta = (DisplayName = "BodyName", MultiLine = "true", Tooltip = "必填项，是绑定驱动身体的骨骼网格体组件的实例"))
		FString BodyName=TEXT("Required");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InitCyanSdk", meta = (DisplayName = "FaceName", MultiLine = "true", Tooltip = "非必填项，是绑定驱动脸部的骨骼网格体组件的实例，主要针对像Metahuman类型的身体和脸部分开的模型"))
		FString FaceName=TEXT("Optional");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InitCyanSdk", meta = (DisplayName = "CyanDataTable", MultiLine = "true", Tooltip = "必填项，是绑定骨骼配置和参数设置的文件"))
		UDataTable* char_DataTable;
	USkeletalMeshComponent* char_Body;
	USkeletalMeshComponent* char_Face;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "InitCyanSdk", meta = (DisplayName = "IP Address", MultiLine = "true", Tooltip = "Please fill in the local IP address of the capturing end. "))
		FString IPaddress="127.0.0.1";
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "InitCyanSdk", meta = (DisplayName = "Cyanhand", MultiLine = "true", Tooltip = "Controls whether Cyan hand snapping is enabled, defaults to true to enable, false to disable."))
		bool  Cyanhand=true;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "InitCyanSdk", meta = (DisplayName = "Loopcount", MultiLine = "true", Tooltip = "The value range is from 1 to 15. ", ClampMin = "0", ClampMax = "15"))
		int Loopcount=5;
	UPROPERTY(Config,EditAnywhere, BlueprintReadWrite, Category = "InitCyanSdk", meta = (DisplayName = "FaceJawSwingWeightRatio", MultiLine = "true", ClampMin = "0.0", ClampMax = "1.0", Tooltip = "The value range is from 0 to 1. "))
		float FaceJawSwingWeightRatio = 1.0f;
	UPROPERTY()
	FVector OriRootPos = FVector(0, 0, 0);
	
	UMocpCharacter();

	void InitializeCharConfig();

	bool IsSupportedEngineVersion(const FString& EngineVersion);

	void ManageConfigFiles(const FString& EngineVersion);

	
public:	
	TArray<FVector> mocpModelTick();
	void  setIK(bool bmocp,TArray<FVector> vbody);
public:
	AMocapHandler* Handleactor = nullptr;
	AMocapBaseApi* BaseAPI = nullptr;
	AMocapBodyApi* BodyAPI = nullptr;
	AMocapLiveHand* handapi = nullptr;
	AMocapLiveFace* faceapi = nullptr;
	AMocapStickMen* stickMen = nullptr;
private:
	class UMocpAnimComponent* MocapComponent;
public:
	// init
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason ) override;
	 void UpdateAnim(USkeletalMeshComponent* mBody, USkeletalMeshComponent* mFace);
	 void SetIKInit();
	 void SetIKName();
	 void NewMocpObject();
	 void PlayerStart();
	 UChildActorComponent* FindChildActorComponentByName(const FName& ComponentName, AActor* Owner);
	 //For substitution
	 UFUNCTION(BlueprintCallable, Category = "CyanSdk",DisplayName="ChangePlayer")
	 void changePlayer(FName modelname);
	 void SetComponentToNull(USkeletalMeshComponent*& Component);
	 void HandleGroomComponents(USkeletalMeshComponent* Component, const TArray<FGroomResource>& GroomResources);
	 void StopAminBPTickandSetOptionType();
	 void Modifycharacterparameters(FJS_Modifycharacterparaneters Parameters);
	 void SetHairAndBodyPhysics();
	 void MocpInit();
	 void InitializeCharacterData();
	 void SetupSkeletonUpdate();
	 void InitializeAnimationComponent();
	 void Freevariate();
	 TArray<FVector> getBodyBank();
	 void EndPlayFreesomething();
	 FVector ConverToHandlerData();
	 static bool GetbShouldReadIni();
	 static void SetbShouldReadIni(bool bsetini);
	 static USkeletalMeshComponent* GetBodySkelmeshCompoent();
	 static FName GetBodySkelmeshPelvisName();
	 static float GetMocpDeltaTime();
	 static void SetDrawBoneOffset(float offsetX, float offsetY);
	 static void SetUseBoneDrawing(bool useboneDraw);
	 static void SetPlaybackSpeed(float speedback);
	 static void SetAssignedKey(FKey assignedkey);
	 // Called every frame
	 virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	 static bool GetOutCamera();

	 void interctTick();

	 UFUNCTION(BlueprintCallable, Category = "CyanSdk")
	 TArray<FVector> GetBoneArrLoc();

	 FVector GetPelvisVec();
	 static UWorld* GetCharWorld();

private:
#if WITH_EDITOR
	void EnsureConfigFileExists();
#endif
    bool CheckBoneNameWrite(FName &ErrorName,int& ErrorIndex);
	bool ReadDataTable(const UDataTable* DataTable);
	void ResetVal();
	bool HasGroomComponentAttached(USkeletalMeshComponent* Component);
	void SaveModelResourceToJson();
	void SaveUserDTToJson();
	void CollectChildActorResources(USkeletalMeshComponent* Component, TArray<FChildActorResource>& OutResources);
    void CollectOtherPartsResources(USkeletalMeshComponent* Component, TArray<FModelPartResource>& OutResources);
	void CollectMaterials(USkeletalMeshComponent* Component, TArray<TSoftObjectPtr<UMaterialInterface>>& Materials);
	FName GetSkeletalMeshName();
private:
	// 轮询逻辑：定时检查按键状态
	void PollKeyboardInput();
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input")
	bool bWasSpaceKeyPressed = false;
	UPROPERTY()
    FTimerHandle PollingTimerHandle;
public:
	UFUNCTION(BlueprintCallable, Category = "CyanSdk", DisplayName = "IsPoseInitialized")
	static bool IsPoseInitialized();

	static FName GetPelvisBoneName ();
	UFUNCTION(BlueprintCallable, Category = "CyanSdk", DisplayName = "AdjustPlaybackSpeed")
	static void AdjustPlaybackSpeed(bool increase);
};
