// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraZoomComponent.generated.h"

USTRUCT(BlueprintType)
struct FIntervalIntEngine
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Min;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Max;
};

class UCameraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STARCRAFT_API UCameraZoomComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	bool IsCameraZooming = false;
	float CameraZoomAlpha = 0.0f;
	int32 CameraZoomIter = 0;
	FVector CameraPastLocation;
	FVector CameraNextLocation;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom")
	float ZoomModifier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom")
	float CameraNextLocationDistanceModifier = 50;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zoom")
	FIntervalIntEngine CameraZoomLimit = { .Min = -4, .Max = 4 };

public:
	UCameraZoomComponent();
	void CameraZoomImplementation(float DeltaTime);
	void UpdateCameraLocationVars(FVector Modifier);

	void OnCameraZoom();
	void OnCameraDecrease();
};
