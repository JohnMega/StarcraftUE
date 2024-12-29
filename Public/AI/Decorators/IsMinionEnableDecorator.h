// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "IsMinionEnableDecorator.generated.h"

UCLASS()
class STARCRAFT_API UIsMinionEnableDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UIsMinionEnableDecorator();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
