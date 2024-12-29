// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MainCameraMovementComponent.h"
#include "Components/CameraZoomComponent.h"
#include "SCGameModeBase.h"
#include "Player/SC_MainCamera.h"

UMainCameraMovementComponent::UMainCameraMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMainCameraMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMainCameraMovementComponent::SetNewActorLocation(float DeltaTime, bool IsMoveOnMouseAvailable)
{
	if (!VelocityVector.IsNearlyZero())
	{
		MoveOnKeyboard(DeltaTime);
	}
	else if (IsMoveOnMouseAvailable)
	{
		//MoveOnMouse(DeltaTime);
	}
}

void UMainCameraMovementComponent::MoveOnKeyboard(float DeltaTime)
{
	UCameraZoomComponent* CameraZoomComponent = GetOwner()->GetComponentByClass<UCameraZoomComponent>();
	if (!CameraZoomComponent) return;

	GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + VelocityVector * DeltaTime * MaxVelocity);
	CameraZoomComponent->UpdateCameraLocationVars(VelocityVector * DeltaTime * MaxVelocity);

	VelocityVector = { 0,0,0 };
}

void UMainCameraMovementComponent::MoveOnMouse(float DeltaTime)
{
	UCameraZoomComponent* CameraZoomComponent = GetOwner()->GetComponentByClass<UCameraZoomComponent>();
	if (!CameraZoomComponent) return;

	FVector2D MousePos;
	Cast<APlayerController>(GetOwner()->GetOwner())->GetMousePosition(MousePos.X, MousePos.Y);
	if (MousePos.X <= 0.0f && MousePos.Y <= 0.0f) return;

	const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

	float ResVelocity = 0.0f;
	FVector2D InputRange;
	FVector2D OutputRange = { 0.0, MaxVelocity };
	if (MousePos.X <= PixelCountMoveOnMouse || MousePos.X >= ViewportSize.X - PixelCountMoveOnMouse)
	{
		InputRange.X = (MousePos.X <= PixelCountMoveOnMouse ? PixelCountMoveOnMouse : ViewportSize.X - PixelCountMoveOnMouse);
		InputRange.Y = (MousePos.X <= PixelCountMoveOnMouse ? 0.0 : ViewportSize.X);
		VelocityVector += (MousePos.X <= PixelCountMoveOnMouse ? FVector{ 0,1,0 } : FVector{ 0,-1,0 });

		ResVelocity = FMath::GetMappedRangeValueClamped(InputRange, OutputRange, MousePos.X);
	}

	if (MousePos.Y <= PixelCountMoveOnMouse || MousePos.Y >= ViewportSize.Y - PixelCountMoveOnMouse)
	{
		InputRange.X = (MousePos.Y <= PixelCountMoveOnMouse ? PixelCountMoveOnMouse : ViewportSize.Y - PixelCountMoveOnMouse);
		InputRange.Y = (MousePos.Y <= PixelCountMoveOnMouse ? 0.0 : ViewportSize.Y);
		VelocityVector += (MousePos.Y <= PixelCountMoveOnMouse ? FVector{ -1,0,0 } : FVector{ 1,0,0 });

		ResVelocity = FMath::GetMappedRangeValueClamped(InputRange, OutputRange, MousePos.Y);
	}

	GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + VelocityVector * DeltaTime * ResVelocity);
	CameraZoomComponent->UpdateCameraLocationVars(VelocityVector * DeltaTime * ResVelocity);

	VelocityVector = { 0,0,0 };
}

void UMainCameraMovementComponent::OnMove(FVector Vect)
{
	VelocityVector = Vect;
}