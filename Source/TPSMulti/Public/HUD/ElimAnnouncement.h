// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ElimAnnouncement.generated.h"

class UHorizontalBox;
class UTextBlock;

UCLASS(Abstract)
class TPSMULTI_API UElimAnnouncement : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* AnnouncementBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AnnouncementText;
	
public:
	void SetElimAnnouncementText(FString AttackerName, FString VictimName) const;
};
