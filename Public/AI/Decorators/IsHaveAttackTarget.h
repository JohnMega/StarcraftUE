// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "IsHaveAttackTarget.generated.h"

UCLASS()
class STARCRAFT_API UIsHaveAttackTarget : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UIsHaveAttackTarget();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
