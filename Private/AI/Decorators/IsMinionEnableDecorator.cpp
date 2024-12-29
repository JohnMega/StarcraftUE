// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/IsMinionEnableDecorator.h"
#include "AI/Characters/Minion/SCMinion.h"
#include "AI/SCAIController.h"

UIsMinionEnableDecorator::UIsMinionEnableDecorator()
{
	NodeName = "Is Minion Enable";
}

bool UIsMinionEnableDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return Cast<ASCMinion>(Cast<ASCAIController>(OwnerComp.GetAIOwner())->GetPawn())->IsInEnableState();
}