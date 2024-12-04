// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MocpStructure.h"
#include "CyanDataTable.h"
#include "CyanDataTable_en.h"

/**
 * 
 */
class CYANSDK_API CyanDataTableInfo
{
public:
	CyanDataTableInfo();
	~CyanDataTableInfo();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (JsonPropertyName = "SkeletalBinding"))
	FJS_ModelskeletonSetting SkeletalBinding;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (JsonPropertyName = "CharacterGlobal_Parameter"))
	FJS_Modifycharacterparaneters CharacterGlobal_Parameter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (JsonPropertyName = "Characterhand_parameters"))
	FHandSettings Characterhand_parameters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (JsonPropertyName = "ModelSettings"))
	FJS_ModelSettings ModelSettings;
	void SetTableInfo(FCyanDataTable* InCyanDataTable, FCyanDataTable_en* InCyanDataTable_en);
	FJS_Modifycharacterparaneters UpdateVal(FCyanDataTable* InCyanDataTable, FCyanDataTable_en* InCyanDataTable_en);
	bool IsSkeletalBindingNotNull();
	bool IsNameNotNull(const FName &InName);
};
