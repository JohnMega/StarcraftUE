// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SpawnPoint/SpawnPointAIController.h"
#include "AI/SpawnPoint//FriendUnitsSpawnPoint.h"

void ASpawnPointAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const auto SpawnPointActor = Cast<AFriendUnitsSpawnPoint>(InPawn);
	if (SpawnPointActor)
	{
		RunBehaviorTree(SpawnPointActor->BehaviorTreeAsset);
	}
}