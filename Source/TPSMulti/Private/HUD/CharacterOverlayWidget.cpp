// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/CharacterOverlayWidget.h"
#include "Components/SizeBox.h"

void UCharacterOverlayWidget::SetAmmoUI(bool ShowUI)
{
	if(!AmmoSB)
	{
		return;
	}
	if(ShowUI)
	{
		AmmoSB->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		AmmoSB->SetVisibility(ESlateVisibility::Collapsed);
	}

}
