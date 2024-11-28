// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/ReturnToMainMenuWidget.h"

#include "MultiplayerSessionsSubsystem.h"
#include "Character/BaseCharacter.h"
#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"

void UReturnToMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UReturnToMainMenuWidget::ReturnButtonClicked()
{
	ReturnButton->SetIsEnabled(false);
	if (const APlayerController* firstPlayerController= GetOwningPlayer())
	{
		ABaseCharacter* baseCharacter=Cast<ABaseCharacter>(firstPlayerController->GetPawn());
		if (baseCharacter)
		{
			baseCharacter->ServerLeaveGame();
			baseCharacter->OnLeftGame.AddDynamic(this,&ThisClass::OnPlayerLeftGame);
		}
		else
		{
			ReturnButton->SetIsEnabled(true);
		}
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UReturnToMainMenuWidget::OnDestroySession(bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		ReturnButton->SetIsEnabled(true);
		return;
	}
	
	const UWorld* world = GetWorld();
	if (world)
	{
		AGameModeBase* gameMode=world->GetAuthGameMode<AGameModeBase>();
		if (gameMode)
		{
			gameMode->ReturnToMainMenuHost();
		}
		else
		{
			if (GetOwningPlayer())
			{
				GetOwningPlayer()->ClientReturnToMainMenuWithTextReason(FText::GetEmpty());
			}
		}
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UReturnToMainMenuWidget::OnPlayerLeftGame()
{
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->DestroySession();
	}
}

void UReturnToMainMenuWidget::MenuSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(true);

	if (GetOwningPlayer())
	{
		FInputModeGameAndUI inputModeData;
		inputModeData.SetWidgetToFocus(TakeWidget());
		GetOwningPlayer()->SetInputMode(inputModeData);
		GetOwningPlayer()->SetShowMouseCursor(true);
	}
	if (ReturnButton)
	{
		ReturnButton->OnClicked.AddUniqueDynamic(this,&ThisClass::ReturnButtonClicked);
	}
	const UGameInstance* gameInstance = GetGameInstance();
	if (gameInstance)
	{
		MultiplayerSessionsSubsystem=gameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		if (MultiplayerSessionsSubsystem)
		{
			MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this,&ThisClass::OnDestroySession);
		}
	}
}

void UReturnToMainMenuWidget::MenuTearDown()
{
	RemoveFromParent();
	if (GetOwningPlayer())
	{
		const FInputModeGameOnly inputModeData;
		GetOwningPlayer()->SetInputMode(inputModeData);
		GetOwningPlayer()->SetShowMouseCursor(false);
	}
	if (ReturnButton)
	{
		ReturnButton->OnClicked.RemoveDynamic(this,&ThisClass::ReturnButtonClicked);
	}
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.RemoveDynamic(this,&ThisClass::OnDestroySession);
	}
}
