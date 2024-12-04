// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../CyanSdk/Public/MetaAnimNode_ApplyMocpData.h"
#include "AnimGraphNode_Base.h"
#include "MetaAnimGraphNode_ApplyMocpData.generated.h"

UCLASS()
class UMetaAnimGraphNode_ApplyMocpData : public UAnimGraphNode_Base
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = Settings)
		FMetaAnimNode_ApplyMocpData Node;
public:

	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType)const override;
	virtual FText GetTooltipText() const override;

	//UAnimGraphNode_Base interface
	virtual FString GetNodeCategory() const override;

};
