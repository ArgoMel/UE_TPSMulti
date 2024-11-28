// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"

class UTexture2D;
class APlayerController;
class UUserWidget;
class UCharacterOverlayWidget;
class UAnnouncementWidget;
class UElimAnnouncement;

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

	UPROPERTY(EditAnywhere)
	TSubclassOf<UElimAnnouncement> ElimAnnouncementClass;

	UPROPERTY(EditAnywhere)
	float ElimAnnouncementTime = 2.5f;

	UFUNCTION()
	void ElimAnnouncementTimerFinished(UElimAnnouncement* MsgToRemove);

	UPROPERTY()
	TArray<UElimAnnouncement*> ElimMessages;

	bool bDrawCrosshair = true;

protected:
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<UUserWidget> CharacterOverlayClass;

	UPROPERTY(EditAnywhere, Category = "Announcements")
	TSubclassOf<UUserWidget> AnnouncementClass;

public:
	UPROPERTY()
	UCharacterOverlayWidget* CharacterOverlay;

	UPROPERTY()
	UAnnouncementWidget* Announcement;

private:
	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor);

public:
	void AddCharacterOverlay();

	void AddAnnouncement();
	void AddElimAnnouncement(FString Attacker, FString Victim);

	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetDrawCrosshair(bool CanDraw) { bDrawCrosshair = CanDraw; }
};
