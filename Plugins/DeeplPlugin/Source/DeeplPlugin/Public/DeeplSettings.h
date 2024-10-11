// Copyright 2023 W³odzimierz Iwanowski. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DeeplSettings.generated.h"

UCLASS(Config = Engine, meta = (DisplayName = "Deepl Translation Plugin"))
class DEEPLPLUGIN_API UDeeplSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	/* This settings will be available in ProjectSettings->DeeplTranslationPlugin */
	
	UDeeplSettings(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Config, EditAnywhere, Category="Deepl Settings")
	FString DeeplAPI;

	UPROPERTY(Config, EditAnywhere, Category="Deepl Settings")
	FString Endpoint;
};
