// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SelectBoxWB.h"
#include "Components/Image.h"
#include "Components/PanelSlot.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/GameViewportSubsystem.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void USelectBoxWB::SelectBoxTick()
{
	FVector2D ResPositionInViewport = BoxCoords[0];
	if ((BoxCoords[1].Y < 0 && BoxCoords[1].X > 0) || (BoxCoords[1].Y > 0 && BoxCoords[1].X < 0))
		ResPositionInViewport = { FMath::Min(BoxCoords[0].X, abs(BoxCoords[1].X)), FMath::Min(BoxCoords[0].Y, abs(BoxCoords[1].Y)) };
	else if (BoxCoords[1].X < 0 && BoxCoords[1].Y < 0)
		ResPositionInViewport = { abs(BoxCoords[1].X), abs(BoxCoords[1].Y) };

	SetPositionInViewport(ResPositionInViewport);
	float DPI = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
	Cast<UCanvasPanelSlot>(SelectBox->Slot)->SetSize({ abs(BoxCoords[0].X - abs(BoxCoords[1].X)) / DPI, abs(BoxCoords[0].Y - abs(BoxCoords[1].Y)) / DPI });
}

void USelectBoxWB::SetBoxStartCoord(FVector2D BoxStartDotCoords)
{
	BoxCoords[0] = BoxStartDotCoords;
}

void USelectBoxWB::SetBoxEndCoord(FVector2D BoxEndDotCoords)
{
	BoxCoords[1] = BoxEndDotCoords;
}

const TArray<FVector2D>& USelectBoxWB::GetBoxCoords() const
{
	return BoxCoords;
}

TArray<FVector2D>& USelectBoxWB::SetBoxCoords()
{
	return BoxCoords;
}

void USelectBoxWB::ClearBoxCoords()
{
	for (int32 i = 0; i < BoxCoords.Num(); ++i)
		BoxCoords[i] = { 0,0 };
}