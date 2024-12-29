// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MainCameraMovementComponent.generated.h"

enum class EMovementType
{
	ON_KEYBOARD,
	ON_MOUSE
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STARCRAFT_API UMainCameraMovementComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	FVector VelocityVector;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxVelocity = 500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	int32 PixelCountMoveOnMouse = 100;

protected:
	virtual void BeginPlay() override;

public:	
	UMainCameraMovementComponent();

	void SetNewActorLocation(float DeltaTime, bool IsMoveOnMouseAvailable);
	void MoveOnKeyboard(float DeltaTime);
	void MoveOnMouse(float DeltaTime);

	void OnMove(FVector Vect);
};
