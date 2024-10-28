// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"

class UTexture2D;
class APlayerController;
class UUserWidget;

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
public:
	TObjectPtr<UTexture2D> CrosshairsCenter;
	TObjectPtr<UTexture2D> CrosshairsLeft;
	TObjectPtr<UTexture2D> CrosshairsRight;
	TObjectPtr<UTexture2D> CrosshairsTop;
	TObjectPtr<UTexture2D> CrosshairsBottom;
	float CrosshairSpread;
	FLinearColor CrosshairsColor;

	FHUDPackage()
	{
		CrosshairSpread = 0.f;
		CrosshairsColor = FLinearColor::White;
	}

	void Clear()
	{
		CrosshairsCenter=nullptr;
		CrosshairsLeft = nullptr;
		CrosshairsRight = nullptr;
		CrosshairsTop = nullptr;
		CrosshairsBottom = nullptr;
	}
};

UCLASS()
class TPSMULTI_API ABaseHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

private:
	UPROPERTY()
	APlayerController* OwningPlayer;

	FHUDPackage HUDPackage;

	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;

	//UPROPERTY(EditAnywhere)
	//TSubclassOf<class UElimAnnouncement> ElimAnnouncementClass;

	UPROPERTY(EditAnywhere)
	float ElimAnnouncementTime = 2.5f;

	//UFUNCTION()
	//void ElimAnnouncementTimerFinished(UElimAnnouncement* MsgToRemove);

	//UPROPERTY()
	//TArray<UElimAnnouncement*> ElimMessages;

protected:
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<UUserWidget> CharacterOverlayClass;

	//UPROPERTY()
	//class UCharacterOverlay* CharacterOverlay;

	UPROPERTY(EditAnywhere, Category = "Announcements")
	TSubclassOf<UUserWidget> AnnouncementClass;

	//UPROPERTY()
	//class UAnnouncement* Announcement;

private:
	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor);

protected:
	void AddCharacterOverlay();

	void AddAnnouncement();
	void AddElimAnnouncement(FString Attacker, FString Victim);

public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; }
};
