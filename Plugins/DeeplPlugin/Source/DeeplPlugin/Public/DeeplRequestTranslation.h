// Copyright 2023 W³odzimierz Iwanowski. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetStringLibrary.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "DeeplRequestTranslation.generated.h"

class FHttpModule;

/* We use these structures to specify input and output variables */

USTRUCT(BlueprintType)
struct FDeeplResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Deepl Response")
	FString TranslatedText;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Deepl Response")
	FString DetectedLanguage;
};

USTRUCT(BlueprintType)
struct FDeeplParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Deepl Parameters", meta = (ToolTip = "used to parse certain text"))
	TArray<FString> Text;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Deepl Parameters", meta = (ToolTip = "set a code that is available in the documentation"))
	FString Source;

	/* We convert response to plain text */
	
	FString GetAsSingleString() const
	{
		const FString MergedString = UKismetStringLibrary::JoinStringArray(Text, "");
		FString FinalString = UKismetStringLibrary::Replace(MergedString," ", "%20", ESearchCase::IgnoreCase);
		return FinalString;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeeplResponse, bool, bSuccess, const FDeeplResponse&, Response);

UCLASS()
class DEEPLPLUGIN_API UDeeplRequestTranslation : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()

public:

	UDeeplRequestTranslation(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable, Category = "DeeplTranslation")
	FOnDeeplResponse OnDeeplResponse;

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "You can overwrite API in function itself", BlueprintInternalUseOnly = "true", AdvancedDisplay = "APIKeyOverride", Category = "DeeplTranslation"))
	static UDeeplRequestTranslation* RequestTranslation(const FDeeplParameters DeeplParameters, const FString APIKeyOverride);

	virtual void Activate() override;

private:

	UPROPERTY()
	FString Endpoint;

	UPROPERTY()
	FString KeyAPI;

	UPROPERTY()
	FDeeplParameters Parameters;

	FHttpModule* Http;

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful) const;
	
};
