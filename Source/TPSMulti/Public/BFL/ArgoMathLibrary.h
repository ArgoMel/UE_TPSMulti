// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ArgoMathLibrary.generated.h"

UCLASS()
class TPSMULTI_API UArgoMathLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "ArgoMathLibrary")
	static float RoundTen(float Src);
	
	UFUNCTION(BlueprintPure, Category = "ArgoMathLibrary")
	static float CeilingTen(float Src);
};
