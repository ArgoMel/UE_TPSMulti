// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/CharacterOverlayWidget.h"
#include "Components/SizeBox.h"
#include "Animation/WidgetAnimation.h"

void UCharacterOverlayWidget::SetAmmoUI(bool ShowUI) const
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

void UCharacterOverlayWidget::PlayBlinkTextAnim(bool PlayAnim)
{
	if(!BlinkTextAnim)
	{
		return;
	}
	if(PlayAnim)
	{
		if (!IsAnimationPlaying(BlinkTextAnim))
		{
			PlayAnimation(BlinkTextAnim);
		}
	}
	else
	{
		if (IsAnimationPlaying(BlinkTextAnim))
		{
			StopAnimation(BlinkTextAnim);
		}
	}
}
