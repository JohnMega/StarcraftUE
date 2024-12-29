// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SpawnPoint/FriendUnitsSpawnPoint.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

AFriendUnitsSpawnPoint::AFriendUnitsSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AFriendUnitsSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector AFriendUnitsSpawnPoint::GetBestSpawnPoint()
{
	auto Blackboard = Cast<AAIController>(GetOwner())->GetBlackboardComponent();
	return Blackboard->GetValueAsVector(SpawnPointKeyName);
}
