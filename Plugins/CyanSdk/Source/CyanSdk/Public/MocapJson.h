// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MocpStructure.h"
#include "CyanDataTableInfo.h"
#include "GroomComponent.h"
#include "Dom/JsonValue.h"
#include "Misc/Paths.h"
#include "CyanDataTableInfo.h"
#include "MocapJson.generated.h"


class UGroomComponent;

UCLASS()
class CYANSDK_API UMocapJson : public UActorComponent
{
	GENERATED_BODY()

		// Sets default values for this actor's properties
		UMocapJson();



public:
	static void InitializeStaticJson();

	//******* BaseApiWritejsonData
	static bool JudgeFileExit(FString jsfilePath);
	static bool JudgeFileExit(FName jsfilePath);
	static void WriteBaseApiFile(FName modelname);
	static void WriteBaseApiFV(FName  modelname, FName  itemname, TArray<FVector> tdata);
	static void WriteBaseApiFQ(FName  modelname, FName  itemname, TArray<FQuat> tdata);
	static void WriteBaseApiTR(FName  modelname, FName  itemname, TArray <FTransform>  tdata);
	static void WriteBaseApiInt(FName  modelname, FName  itemname, int  tdata);
	static void WriteBaseApiTransform(FName  modelname, FName  itemname, FTransform tdata);
	static void WriteBaseApiFVector(FName  modelname, FName  itemname, FVector tdata);
	static void WriteBaseApiDouble(FName modelname, FName itemname, double tdata);
	static void WriteBaseApiTrfloat(FName modelname, FName itemname, TArray<float> tdata);
	static void WriteBaseApiTrFN(FName modelname, FName itemname, TArray<FName> tdata);
	static void WriteBaseApiFN(FName modelname, FName itemname, FName tdata);
	static void SaveJsonFile();
	//****** BaseApiGetjsonData
	static TArray<FVector> GetBaseApiFV(FName  modelname, FName  itemname);
	static TArray<FQuat> GetBaseApiFQ(FName  modelname, FName  itemname);
	static TArray <FTransform> GetBaseApiTR(FName  modelname, FName  itemname);
	static int GetBaseApiInt(FName  modelname, FName  itemname);
	static  FTransform GetBaseApiTransform(FName  modelname, FName  itemname);
	static FVector GetBaseApiFVector(FName  modelname, FName  itemname);
	static double GetBaseApiDouble(FName  modelname, FName  itemname);
	static TArray<float>GetBaseApiTrfloat(FName  modelname, FName  itemname);
	static TArray<FName>GetBaseApiTrFN(FName  modelname, FName  itemname);
	static FName GetBaseApiFN(FName  modelname, FName  itemname);

	//****** Model resource loading JsonData

	static FString GetBaseApiString(FName modelname, FName itemname);

	static FJs_ModelSelect ReadModleSelect();

	



	static void SetModelName(FName name);
	

	static FName GetcyansysFileName(int index);

	static FModelResource LoadModelResourceFromJson(FName modelname);

	static void SaveStructTojson(bool IsDT, const UStruct* StructDefinition, const void* Struct, int64 CheckFlags, int64 SkipFlags);

	static void LoadSkeletalMesh(USkeletalMeshComponent* Component, TSoftObjectPtr<USkeletalMesh> Mesh, TSoftClassPtr<UAnimInstance> AnimBP);

	static void LoadAnimationBlueprint(USkeletalMeshComponent* Component, TSoftClassPtr<UAnimInstance> AnimBP);

	static void ApplyMaterials(UMeshComponent* Component, const TArray<TSoftObjectPtr<UMaterialInterface>>& Materials);

	static void OnMaterialsLoaded(UMeshComponent* Component, const TArray<TSoftObjectPtr<UMaterialInterface>>& Materials);

	static void LoadGroomResource(UGroomComponent* grComponent,const FGroomResource& GroomResource);

	static USkeletalMeshComponent* FindPartComponentByName(const FName& PartName, USkeletalMeshComponent* skComP);

	static UGroomComponent* FindGroomComponentByName(const FName& PartName, TArray<UGroomComponent*>& GrComs);
	static FCyanDataTable LoadUserDTFromJson(FName modelname);
	static CyanDataTableInfo CyanDataTableToInfo(FCyanDataTable& cyandatadt);

	static void SetModelNameToIndexMap(FName Modelname);

	static void ClearModelNameToIndexMap();

	static bool GetIsChangePlayer();
	static void SetCacheEnabled(bool isset);
	static bool GetCacheEnabled();
	static void SetIschangePlayer(bool setchange);
	

	static bool MergeJsonStringsAndSave(const FString& JsonString1, const FString& JsonString2, const FString& JsonString3, const FString& SavePath);

	//Judge
	static FString GetBonebindingFileName(FString modelname);

	static FJS_ModelhandSettings GetModelhandSettings(int id);
	static int GetPluginsVersion();
	static FString GetPluginVersion();
	static void CheckjsondataVersion(FName modelname);
	static int GetIniLoopcount();
	static float GetIniFaceJawSwingWeightRatio();
	static bool GetCyanhand();

	static bool CheckModelCacheExist(FName modelname);
	static bool CheckModelDataCorrect(FTransform& tf, FTransform& tf2);
};
