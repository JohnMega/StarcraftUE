// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "IsUnitSelectedDecorator.generated.h"

UCLASS()
class STARCRAFT_API UIsUnitSelectedDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UIsUnitSelectedDecorator();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
