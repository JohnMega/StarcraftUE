// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/RaynorDescription/StimpackDescriptionWB.h"
#include "MediaPlayer.h"
#include "FileMediaSource.h"
#include "Animation/WidgetAnimation.h"

void UStimpackDescriptionWB::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	if (InVisibility == ESlateVisibility::Hidden && IsStimpackButtonPressed)
	{
		OnStimpackPressed();
	}
}

void UStimpackDescriptionWB::OnStimpackPressed()
{
	if (IsStimpackButtonPressed)
	{
		PlayAnimationReverse(StimpackDescriptionVisibilityAnim);
		StimpackMediaPlayer->Close();
		IsStimpackButtonPressed = false;
	}
	else
	{
		PlayAnimation(StimpackDescriptionVisibilityAnim);
		StimpackMediaPlayer->OpenSource(StimpackMediaSource);

		FVector2D CursorPosition;
		(*GetWorld()->GetPlayerControllerIterator())->GetMousePosition(CursorPosition.X, CursorPosition.Y);
		SetPositionInViewport(CursorPosition);

		IsStimpackButtonPressed = true;
	}
}