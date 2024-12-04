// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include"MocpStructure.h"
#include "CyanDataTable.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct CYANSDK_API FCyanDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CyanDataTable", meta = (JsonPropertyName = "SkeletalBinding", DisplayName = "模型骨骼绑定", MultiLine = "true", Tooltip = "可以给相关模型绑定骨骼"))
		FJS_ModelskeletonSetting SkeletalBinding;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CyanDataTable", meta = (JsonPropertyName = "CharacterGlobal_Parameter",DisplayName = "人物全身参数调整", MultiLine = "true", Tooltip = "可以调整离地高度，人物脚踝旋转，两腿间距，是否强制人物进行初始化，防止拉扯小腿，肘关节旋转比率，调整肩膀凹陷。"))
		FJS_Modifycharacterparaneters CharacterGlobal_Parameter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CyanDataTable", meta = (JsonPropertyName = "Characterhand_parameters",DisplayName = "人物手部参数调整", MultiLine = "true", Tooltip = "可以调整模型手指外翻，手指握拳的旋转角度，手指水平移动时的角度"))
		FHandSettings Characterhand_parameters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CyanDataTable", meta = (JsonPropertyName = "ModelSettings",DisplayName = "控制动画蓝图刷新", MultiLine = "true", Tooltip = "可以使用手动骨骼刷新和调整渲染模式 减少额外的性能，但可能会使额外的动画蓝图插件不兼容"))
		FJS_ModelSettings ModelSettings;

	
	FJS_ModelskeletonSetting GetModelskeletonSettingInfo();
	FJS_Modifycharacterparaneters GetModifycharacterparanetersInfo();
	FHandSettings GetHandSettingsInfo();
	FJS_ModelSettings GetModelSettingsInfo();


};
