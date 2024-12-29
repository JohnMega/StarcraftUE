// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FriendUnitsSpawnPoint.generated.h"

class UBehaviorTree;
class UUnitSelectMenuWB;

UCLASS()
class STARCRAFT_API AFriendUnitsSpawnPoint : public APawn
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blackboard")
	FName SpawnPointKeyName = "SpawnPoint";

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behavior Tree")
	UBehaviorTree* BehaviorTreeAsset;

protected:
	virtual void BeginPlay() override;

public:	
	AFriendUnitsSpawnPoint();

	FVector GetBestSpawnPoint();
};
