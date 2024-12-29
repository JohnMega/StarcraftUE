// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ClearAllGoalActors.generated.h"

UCLASS()
class STARCRAFT_API UClearAllGoalActors : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UClearAllGoalActors();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
