// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimGraphNode_Base.h"
#include "../../CyanSdk/Public/MocpFaceModifyCurveData.h"
#include "MocpGraphFaceModifyCurveData.generated.h"

/**
 *
 */
 /** Easy way to modify curve values on a pose */
UCLASS(MinimalAPI)
class UMocpGraphFaceModifyCurveData : public UAnimGraphNode_Base
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, Category = Settings)
		FMocpFaceModifyCurveData Node;
public:

	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType)const override;
	virtual FText GetTooltipText() const override;

	//UAnimGraphNode_Base interface
	virtual FString GetNodeCategory() const override;

};
