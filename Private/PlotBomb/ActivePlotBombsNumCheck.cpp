// Fill out your copyright notice in the Description page of Project Settings.


#include "PlotBomb/ActivePlotBombsNumCheck.h"
#include "PlotBomb/PlotBomb.h"

AActivePlotBombsNumCheck::AActivePlotBombsNumCheck()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AActivePlotBombsNumCheck::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(ActiveBombsNumCheckTimer, this, &AActivePlotBombsNumCheck::ActiveBombsNumCheck, 0.2f, true);
}

void AActivePlotBombsNumCheck::ActiveBombsNumCheck()
{
	bool IsAllBombsActive = true;
	for (int32 i = 0; i < CheckedPlotBombs.Num(); ++i)
	{
		if (!CheckedPlotBombs[i]->IsBombActive())
		{
			IsAllBombsActive = false;
			break;
		}
	}

	if (IsAllBombsActive)
	{
		OnAllBombsActivate.Broadcast();
		GetWorld()->GetTimerManager().ClearTimer(ActiveBombsNumCheckTimer);
		UE_LOG(LogTemp, Error, TEXT("All bombs active"));
	}
}