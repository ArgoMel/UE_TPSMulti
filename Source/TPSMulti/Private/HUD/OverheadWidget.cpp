// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/OverheadWidget.h"
#include "Components/TextBlock.h"

void UOverheadWidget::NativeDestruct()
{
	RemoveFromParent();
	Super::NativeDestruct();
}

void UOverheadWidget::SetDisplayText(FString TextToDisplay)
{
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}
}

void UOverheadWidget::ShowPlayerNetRole(APawn* InPawn)
{
	ENetRole remoteRole = InPawn->GetRemoteRole();
	FString remoteRoleString = FString::Printf(TEXT("Remote Role: %s"), *UEnum::GetDisplayValueAsText(remoteRole).ToString());
	SetDisplayText(remoteRoleString);
}
