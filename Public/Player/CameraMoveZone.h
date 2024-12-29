// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraMoveZone.generated.h"

class UStaticMeshComponent;
class UChildActorComponent;

UCLASS()
class STARCRAFT_API ACameraMoveZone : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* CameraMoveZone;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UChildActorComponent* MoveZoneDotMinusX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UChildActorComponent* MoveZoneDotPlusX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UChildActorComponent* MoveZoneDotMinusY;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UChildActorComponent* MoveZoneDotPlusY;

public:	
	ACameraMoveZone();

	FVector GetMoveZoneDotMinusX() const;
	FVector GetMoveZoneDotPlusX() const;
	FVector GetMoveZoneDotMinusY() const;
	FVector GetMoveZoneDotPlusY() const;
	FVector GetUpYVector() const;
	FVector GetUpXVector() const;
	FVector GetUpZVector() const;
};
