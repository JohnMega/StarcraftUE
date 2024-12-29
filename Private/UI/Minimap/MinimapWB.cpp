// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Minimap/MinimapWB.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "UI/Minimap/Minimap.h"
#include "Blueprint/WidgetLayoutLibrary.h"

bool UMinimapWB::Initialize()
{
	bool InitRes = Super::Initialize();

	if (MinimapMoveButton)
		MinimapMoveButton->OnClicked.AddDynamic(this, &UMinimapWB::OnMinimapMoveButtonClicked);

	return InitRes;
}

void UMinimapWB::OnMinimapMoveButtonClicked()
{
	auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	FVector2D MousePosition;
	TInterval<int32> ViewportSize;
	PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);
	PlayerController->GetViewportSize(ViewportSize.Min, ViewportSize.Max);

	auto Minimap = Cast<AMinimap>(UGameplayStatics::GetActorOfClass(GetWorld(), MinimapClass.Get()));
	float OnePixelLenght = (Minimap->GetMinimapPlaneHalfLenght() * 2) / MinimapImage->GetBrush().GetImageSize().X;

	float DPI = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
	FVector2D NewMousePosition = { MousePosition.X / DPI, MinimapImage->GetBrush().GetImageSize().Y - (ViewportSize.Max - MousePosition.Y) / DPI };
	
	UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->SetActorLocation(
		{
		Minimap->GetMinimapPlaneMoveStartDot()->GetComponentLocation().X + OnePixelLenght * NewMousePosition.X,
		Minimap->GetMinimapPlaneMoveStartDot()->GetComponentLocation().Y + OnePixelLenght * NewMousePosition.Y,
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().Z
		});
}