// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MocpStructure.h"
#include "CyanDataTable.h"
#include "CyanDataTable_en.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct CYANSDK_API FCyanDataTable_en : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CyanDataTable", meta = (JsonPropertyName = "SkeletalBinding",DisplayName = "Model Binding", MultiLine = "true", Tooltip = "Bind the relevant models to the corresponding bone nodes."))
	FJS_ModelskeletonSetting_E SkeletalBinding;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CyanDataTable", meta = (JsonPropertyName = "CharacterGlobal_Parameter", DisplayName = "Character full-body parameter adjustment", MultiLine = "true", Tooltip = "Customize the character model parameters, which if adjusted appropriately can improve the accuracy of the motion capture"))
	FJS_Modifycharacterparaneters_E CharacterGlobal_Parameter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CyanDataTable", meta = (JsonPropertyName = "Characterhand_parameters", DisplayName = "Character hand parameter adjustment", MultiLine = "true", Tooltip = "It can be used to manually adjust the hand after it has developed grip problems"))
	FHandSettings_E Characterhand_parameters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CyanDataTable", meta = (JsonPropertyName = "ModelSettings", DisplayName = "Control animated blueprint refresh.", MultiLine = "true", Tooltip = "Can use manual bone refresh and adjust rendering modes Reduces additional performance consumption, but may make additional animation blueprint plugins incompatible"))
	FJS_ModelSettings_E ModelSettings;

	FJS_ModelskeletonSetting GetSkeletalBindingInfo();
	FJS_Modifycharacterparaneters GetModifycharacterparanetersInfo();
	FHandSettings GetHandSettingsInfo();
	FJS_ModelSettings GetModelSettingsInfo();
	FCyanDataTable CyanDataTable_entoCyanDataTable(FCyanDataTable_en* cyandatadten);
};