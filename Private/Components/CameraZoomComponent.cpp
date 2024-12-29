// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CameraZoomComponent.h"
#include "Camera/CameraComponent.h"

UCameraZoomComponent::UCameraZoomComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCameraZoomComponent::CameraZoomImplementation(float DeltaTime)
{
	if (!IsCameraZooming) return;
	if (!(CameraZoomIter >= CameraZoomLimit.Min && CameraZoomIter <= CameraZoomLimit.Max))
	{
		IsCameraZooming = false;
		return;
	}

	UCameraComponent* CameraComponent = GetOwner()->GetComponentByClass<UCameraComponent>();
	if (!CameraComponent) return;

	CameraZoomAlpha = FMath::Clamp(CameraZoomAlpha + ZoomModifier * DeltaTime, 0.0f, 1.0f);
	CameraComponent->SetWorldLocation(FMath::Lerp(CameraPastLocation, CameraNextLocation, CameraZoomAlpha));

	if (CameraZoomAlpha >= 1.0f)
	{
		IsCameraZooming = false;
		CameraZoomAlpha = 0.0f;
	}
}

void UCameraZoomComponent::UpdateCameraLocationVars(FVector Modifier)
{
	CameraNextLocation += Modifier;
	CameraPastLocation += Modifier;
}

void UCameraZoomComponent::OnCameraZoom()
{
	if (IsCameraZooming) return;

	UCameraComponent* CameraComponent = GetOwner()->GetComponentByClass<UCameraComponent>();
	if (!CameraComponent) return;

	IsCameraZooming = true;
	CameraPastLocation = CameraComponent->GetComponentLocation();
	CameraNextLocation = CameraComponent->GetComponentLocation() + CameraComponent->GetForwardVector() * CameraNextLocationDistanceModifier;

	CameraZoomIter = FMath::Clamp(CameraZoomIter + (CameraZoomIter == CameraZoomLimit.Min - 1 ? 2 : 1), CameraZoomLimit.Min - 1, CameraZoomLimit.Max + 1);
}

void UCameraZoomComponent::OnCameraDecrease()
{
	if (IsCameraZooming) return;

	UCameraComponent* CameraComponent = GetOwner()->GetComponentByClass<UCameraComponent>();
	if (!CameraComponent) return;

	IsCameraZooming = true;
	CameraPastLocation = CameraComponent->GetComponentLocation();
	CameraNextLocation = CameraComponent->GetComponentLocation() - CameraComponent->GetForwardVector() * CameraNextLocationDistanceModifier;

	CameraZoomIter = FMath::Clamp(CameraZoomIter - (CameraZoomIter == CameraZoomLimit.Max + 1 ? 2 : 1), CameraZoomLimit.Min - 1, CameraZoomLimit.Max + 1);
}