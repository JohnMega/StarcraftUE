// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Units/KeyboardButtonWB.h"
#include "Animation/WidgetAnimation.h"

void UKeyboardButtonWB::SetVisibility(ESlateVisibility InVisibility)
{
	if (InVisibility == ESlateVisibility::Visible && LATPWidgetVisibleAnim == ELastAnimationTypePlayed::REVERSE)
	{
		PlayAnimation(WidgetVisibleAnim);
		LATPWidgetVisibleAnim = ELastAnimationTypePlayed::DEFAULT;
	}
	else if (InVisibility == ESlateVisibility::Hidden && LATPWidgetVisibleAnim == ELastAnimationTypePlayed::DEFAULT)
	{
		PlayAnimationReverse(WidgetVisibleAnim);
		LATPWidgetVisibleAnim = ELastAnimationTypePlayed::REVERSE;
	}
}