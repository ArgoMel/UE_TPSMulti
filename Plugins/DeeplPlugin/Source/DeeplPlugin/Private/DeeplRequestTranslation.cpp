// Copyright 2023 W³odzimierz Iwanowski. All Rights Reserved.

#include "DeeplRequestTranslation.h"
#include "DeeplSettings.h"
#include "Kismet/KismetStringLibrary.h"

/* set configs from our developer settings */

UDeeplRequestTranslation::UDeeplRequestTranslation(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	Http = &FHttpModule::Get();
	const UDeeplSettings* DeeplConfig = GetDefault<UDeeplSettings>();
	KeyAPI = DeeplConfig->DeeplAPI;
	Endpoint = DeeplConfig->Endpoint;
}

/* if we specified APIKey in function we will overwrite our project settings */

UDeeplRequestTranslation* UDeeplRequestTranslation::RequestTranslation(const FDeeplParameters DeeplParameters, 	const FString APIKeyOverride)
{
	UDeeplRequestTranslation* Receiver = NewObject<UDeeplRequestTranslation>();
	Receiver->Parameters = DeeplParameters;

	if (!APIKeyOverride.IsEmpty())
	{
		Receiver->KeyAPI = APIKeyOverride;
	}
	return Receiver;
}

/* we send our parameters to get response from Deepl */

void UDeeplRequestTranslation::Activate()
{
	const FString FinalAPI = "?auth_key=" + KeyAPI;
	const FString SetTranslation = "&text=" + Parameters.GetAsSingleString();
	const FString SetLang = "&target_lang=" + Parameters.Source;
	const FString FinalURL = Endpoint + FinalAPI + SetTranslation + SetLang;
	const FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UDeeplRequestTranslation::OnResponseReceived);
	Request->SetURL(FinalURL);
	UE_LOG(LogTemp, Error, TEXT("Payload %s"), *FinalURL)
	Request->SetVerb("GET");
	Request->ProcessRequest();
}

/* We receive response if it's 200 we send our resulet to DeeplResponse structure, if not we print out our code error */

void UDeeplRequestTranslation::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful) const
{
	FDeeplResponse LocalDeeplResponse;
	
	const bool bSuccessResponse = Response->GetResponseCode() == 200;
	if (bSuccessResponse)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Response is %s"), *Response->GetContentAsString()); <- Entire Request
		FString DetectionLanguage = UKismetStringLibrary::GetSubstring(Response->GetContentAsString(), 46,2);
		FString Translation = UKismetStringLibrary::GetSubstring(Response->GetContentAsString(), 58, Response->GetContentAsString().Len()-62);
		LocalDeeplResponse.DetectedLanguage = DetectionLanguage;
		LocalDeeplResponse.TranslatedText = Translation;
	}
	
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Response Code: %d"), Response->GetResponseCode())
	}
	
	OnDeeplResponse.Broadcast(bSuccessResponse, LocalDeeplResponse);
}