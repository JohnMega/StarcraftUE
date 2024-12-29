// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GoalMask/GoalMaskWB.h"
#include "Components/Image.h"
#include "Player/SC_MainCamera.h"
#include "Kismet/GameplayStatics.h"

void UGoalMaskWB::InitVars(FGoalMaskInitVars NewGoalMaskInitVars)
{
	GoalMaskInitVars = NewGoalMaskInitVars;
}

void UGoalMaskWB::ProjectWorldToScreenWithOutsideViewPos(APlayerController const* Player, const FVector& WorldPosition, FVector2D& ScreenPosition)
{
	ULocalPlayer* const LP = Player ? Player->GetLocalPlayer() : nullptr;
	if (LP && LP->ViewportClient)
	{
		// get the projection data
		FSceneViewProjectionData ProjectionData;
		if (LP->GetProjectionData(LP->ViewportClient->Viewport, /*out*/ ProjectionData))
		{
			FMatrix const ViewProjectionMatrix = ProjectionData.ComputeViewProjectionMatrix();
			FSceneView::ProjectWorldToScreen(WorldPosition, ProjectionData.GetConstrainedViewRect(), ViewProjectionMatrix, ScreenPosition, true);
		}
	}
}