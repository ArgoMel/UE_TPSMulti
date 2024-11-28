// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReturnToMainMenuWidget.generated.h"

class UButton;
class UMultiplayerSessionsSubsystem;

UCLASS(Abstract)
class TPSMULTI_API UReturnToMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY(meta = (BindWidget))
	UButton* ReturnButton;

	UPROPERTY()
	UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

private:
	UFUNCTION()
	void ReturnButtonClicked();
	
protected:
	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);

	UFUNCTION()
	void OnPlayerLeftGame();
	
public:
	void MenuSetup();
	void MenuTearDown();
};
