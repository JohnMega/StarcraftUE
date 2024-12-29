// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/ClearAllGoalActors.h"
#include "AIController.h"
#include "AI/SCAICharacter.h"

UClearAllGoalActors::UClearAllGoalActors()
{
	NodeName = "Clear Current Goal Actor";
}

EBTNodeResult::Type UClearAllGoalActors::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!GetWorld()) return EBTNodeResult::Failed;

	auto Character = Cast<ASCAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if(!Character) return EBTNodeResult::Failed;

	Character->SetCurrentGoal(nullptr);
	return EBTNodeResult::Succeeded;
}