// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/BaseHUD.h"
#include "HUD/CharacterOverlayWidget.h"
#include "HUD/AnnouncementWidget.h"
//#include "ElimAnnouncement.h
#include "GameFramework/PlayerController.h"
#include "Components/HorizontalBox.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"

void ABaseHUD::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseHUD::DrawHUD()
{
	Super::DrawHUD();
	FVector2D viewportSize;
	if(GEngine)
	{
		GEngine->GameViewport->GetViewportSize(viewportSize);
		const FVector2D viewportCenter(viewportSize.X/2.f,viewportSize.Y/2.f);
		float spreadScaled = CrosshairSpreadMax * HUDPackage.CrosshairSpread;

		if(HUDPackage.CrosshairsCenter)
		{
			DrawCrosshair(HUDPackage.CrosshairsCenter, viewportCenter, FVector2D::ZeroVector, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsLeft)
		{
			FVector2D spread(-spreadScaled, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsLeft, viewportCenter, spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsRight)
		{
			FVector2D spread(spreadScaled, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsRight, viewportCenter, spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsTop)
		{
			FVector2D spread(0.f, -spreadScaled);
			DrawCrosshair(HUDPackage.CrosshairsTop, viewportCenter, spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsBottom)
		{
			FVector2D spread(0.f, spreadScaled);
			DrawCrosshair(HUDPackage.CrosshairsBottom, viewportCenter, spread, HUDPackage.CrosshairsColor);
		}
	}
}

void ABaseHUD::DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor)
{
	const float textureWidth = Texture->GetSizeX();
	const float textureHeight = Texture->GetSizeY();
	const FVector2D textureDrawPoint(ViewportCenter.X - (textureWidth / 2.f)+ Spread.X, ViewportCenter.Y - (textureHeight / 2.f) + Spread.Y);
	DrawTexture(Texture, textureDrawPoint.X, textureDrawPoint.Y, textureWidth, textureHeight,0.f,0.f,1.f,1.f, CrosshairColor);
}

void ABaseHUD::AddCharacterOverlay()
{
	APlayerController* playerController = GetOwningPlayerController();
	if (playerController && CharacterOverlayClass)
	{
		CharacterOverlay = CreateWidget<UCharacterOverlayWidget>(playerController, CharacterOverlayClass);
		CharacterOverlay->AddToViewport();
	}
}

void ABaseHUD::AddAnnouncement()
{
	APlayerController* playerController = GetOwningPlayerController();
	if (playerController && AnnouncementClass)
	{
		Announcement = CreateWidget<UAnnouncementWidget>(playerController, AnnouncementClass);
		Announcement->AddToViewport();
	}
}

void ABaseHUD::AddElimAnnouncement(FString Attacker, FString Victim)
{
}
