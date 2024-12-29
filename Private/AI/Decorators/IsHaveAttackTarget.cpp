// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/IsHaveAttackTarget.h"
#include "AIController.h"
#include "AI/SCAICharacter.h"

UIsHaveAttackTarget::UIsHaveAttackTarget()
{
	NodeName = "Is Have Attack Target";
}

bool UIsHaveAttackTarget::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return false;

	ASCAICharacter* Character = Cast<ASCAICharacter>(Controller->GetCharacter());
	if (!Character) return false;

	return Character->IsHaveAttackTarget();
}
