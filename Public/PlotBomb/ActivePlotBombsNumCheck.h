// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActivePlotBombsNumCheck.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllBombsActivateSignature);

class APlotBomb;

UCLASS()
class STARCRAFT_API AActivePlotBombsNumCheck : public AActor
{
	GENERATED_BODY()

private:
	FTimerHandle ActiveBombsNumCheckTimer;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Plot Bombs")
	TArray<APlotBomb*> CheckedPlotBombs;

public:
	UPROPERTY(BlueprintAssignable)
	FOnAllBombsActivateSignature OnAllBombsActivate;

private:
	void ActiveBombsNumCheck();
	
protected:
	virtual void BeginPlay() override;

public:	
	AActivePlotBombsNumCheck();
};
