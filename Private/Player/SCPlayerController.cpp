// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SCPlayerController.h"
#include "AI/SCGoalActor.h"

void ASCPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	bShowMouseCursor = true;
}