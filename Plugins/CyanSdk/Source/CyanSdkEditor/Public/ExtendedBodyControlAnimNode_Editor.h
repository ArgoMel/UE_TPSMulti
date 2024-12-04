// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimGraphNode_Base.h"
#include  "../../CyanSdk/Public/ExtendedBodyControlAnimNode.h"
#include "ExtendedBodyControlAnimNode_Editor.generated.h"

/**
 * 
 */
UCLASS(MinimalAPI)
class  UExtendedBodyControlAnimNode_Editor : public UAnimGraphNode_Base
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Settings)
	FExtendedBodyControlAnimNode Node;
public:
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType)const override;
	virtual FText GetTooltipText() const override;

	//UAnimGraphNode_Base interface
	virtual FString GetNodeCategory() const override;
};
