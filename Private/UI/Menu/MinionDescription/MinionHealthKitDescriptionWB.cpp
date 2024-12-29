// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/MinionDescription/MinionHealthKitDescriptionWB.h"
#include "MediaPlayer.h"
#include "FileMediaSource.h"
#include "Animation/WidgetAnimation.h"

void UMinionHealthKitDescriptionWB::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	if (InVisibility == ESlateVisibility::Hidden && IsHealthKitButtonPressed)
	{
		OnMinionHealthKitPressed();
	}
}

void UMinionHealthKitDescriptionWB::OnMinionHealthKitPressed()
{
	if (IsHealthKitButtonPressed)
	{
		PlayAnimationReverse(HealthKitDescriptionVisibilityAnim);
		HealthKitMediaPlayer->Close();
		IsHealthKitButtonPressed = false;
	}
	else
	{
		PlayAnimation(HealthKitDescriptionVisibilityAnim);
		HealthKitMediaPlayer->OpenSource(HealthKitMediaSource);

		FVector2D CursorPosition;
		(*GetWorld()->GetPlayerControllerIterator())->GetMousePosition(CursorPosition.X, CursorPosition.Y);
		SetPositionInViewport(CursorPosition);

		IsHealthKitButtonPressed = true;
	}
}
