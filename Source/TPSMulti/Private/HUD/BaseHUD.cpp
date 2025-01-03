// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/BaseHUD.h"

#include "BFL/ArgoMathLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/HorizontalBox.h"
#include "Components/OverlaySlot.h"
#include "Components/TextBlock.h"
#include "HUD/CharacterOverlayWidget.h"
#include "HUD/AnnouncementWidget.h"
#include "GameFramework/PlayerController.h"
#include "HUD/ElimAnnouncement.h"
#include "TPSMulti/TPSMulti.h"

constexpr float MsgOffset=20.f;

void ABaseHUD::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseHUD::DrawHUD()
{
	Super::DrawHUD();
	FVector2D viewportSize;
	if(GEngine&& bDrawCrosshair)
	{
		GEngine->GameViewport->GetViewportSize(viewportSize);
		const FVector2D viewportCenter(viewportSize.X/2.f,viewportSize.Y/2.f);
		const float spreadScaled = CrosshairSpreadMax * HUDPackage.CrosshairSpread;

		if(HUDPackage.CrosshairsCenter)
		{
			DrawCrosshair(HUDPackage.CrosshairsCenter, viewportCenter, FVector2D::ZeroVector, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsLeft)
		{
			const FVector2D spread(-spreadScaled, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsLeft, viewportCenter, spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsRight)
		{
			const FVector2D spread(spreadScaled, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsRight, viewportCenter, spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsTop)
		{
			const FVector2D spread(0.f, -spreadScaled);
			DrawCrosshair(HUDPackage.CrosshairsTop, viewportCenter, spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsBottom)
		{
			const FVector2D spread(0.f, spreadScaled);
			DrawCrosshair(HUDPackage.CrosshairsBottom, viewportCenter, spread, HUDPackage.CrosshairsColor);
		}
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ABaseHUD::ElimAnnouncementTimerFinished(UElimAnnouncement* MsgToRemove)
{
	if (MsgToRemove)
	{
		MsgToRemove->RemoveFromParent();
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
		CharacterOverlay->AddToViewport(TopHud);
	}
}

void ABaseHUD::AddAnnouncement()
{
	APlayerController* playerController = GetOwningPlayerController();
	if (playerController && AnnouncementClass)
	{
		Announcement = CreateWidget<UAnnouncementWidget>(playerController, AnnouncementClass);
		Announcement->AddToViewport(TopHud);
	}
}

void ABaseHUD::AddElimAnnouncement(FString Attacker, FString Victim)
{
	APlayerController* playerController = GetOwningPlayerController();
	if (playerController && ElimAnnouncementClass)
	{
		UElimAnnouncement* elimAnnouncementWidget=CreateWidget<UElimAnnouncement>(playerController, ElimAnnouncementClass);
		if (elimAnnouncementWidget)
		{
			elimAnnouncementWidget->SetElimAnnouncementText(Attacker,Victim);
			elimAnnouncementWidget->AddToViewport(TopHud);

			for (const auto& msg:ElimMessages)
			{
				if (msg&&
					msg->AnnouncementBox)
				{
					UOverlaySlot* overlaySlot=UWidgetLayoutLibrary::SlotAsOverlaySlot(msg->AnnouncementBox);
					if (overlaySlot)
					{
						FMargin padding=overlaySlot->GetPadding();
						padding.Bottom+=UArgoMathLibrary::CeilingTen(msg->AnnouncementText->GetFont().Size+MsgOffset);
						overlaySlot->SetPadding(padding);
					}
				}
			}
			
			ElimMessages.Add(elimAnnouncementWidget);

			FTimerHandle elimMsgTimer;
			FTimerDelegate elimMsgDel;
			elimMsgDel.BindUFunction(this,FName("ElimAnnouncementTimerFinished"),elimAnnouncementWidget);
			GetWorldTimerManager().SetTimer(elimMsgTimer,elimMsgDel,ElimAnnouncementTime,false);
		}
	}
}
