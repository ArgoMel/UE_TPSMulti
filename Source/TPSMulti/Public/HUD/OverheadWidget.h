// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverheadWidget.generated.h"

UCLASS(Abstract)
class TPSMULTI_API UOverheadWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeDestruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DisplayText;

public:
	UFUNCTION(BlueprintCallable)
	void SetDisplayText(FString TextToDisplay);

	UFUNCTION(BlueprintCallable)
	void ShowPlayerNetRole(APawn* InPawn);
};
