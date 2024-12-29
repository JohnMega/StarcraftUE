// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SpawnPointAIController.generated.h"

UCLASS()
class STARCRAFT_API ASpawnPointAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	void OnPossess(APawn* InPawn) override;
};
