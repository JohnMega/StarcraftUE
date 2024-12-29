// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/IsFriendlyDecorator.h"
#include "AIController.h"
#include "AI/SCAICharacter.h"

bool UIsFriendlyDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto Controller = OwnerComp.GetAIOwner();
	if (!Controller) return false;

	auto Character = Cast<ASCAICharacter>(Controller->GetCharacter());
	if (!Character) return false;

	return Character->IsFriendly();
}