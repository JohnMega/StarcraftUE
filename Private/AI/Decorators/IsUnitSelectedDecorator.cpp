// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/IsUnitSelectedDecorator.h"
#include "AIController.h"
#include "AI/SCAICharacter.h"

UIsUnitSelectedDecorator::UIsUnitSelectedDecorator()
{
	NodeName = "Is Unit Selected";
}

bool UIsUnitSelectedDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return false;

	ASCAICharacter* Character = Cast<ASCAICharacter>(Controller->GetCharacter());
	if (!Character) return false;

	return (!Character->IsFriendly() ? true : Character->IsSelected());
}