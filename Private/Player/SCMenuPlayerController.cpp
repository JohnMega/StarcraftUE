// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SCMenuPlayerController.h"
#include "Kismet/GameplayStatics.h"

ASCMenuPlayerController::ASCMenuPlayerController()
{
	bShowMouseCursor = true;
}

void ASCMenuPlayerController::BeginPlay()
{
	AActor* CurrentPawnOnScene = UGameplayStatics::GetActorOfClass(GetWorld(), CurrentPawnOnSceneClass.Get());
	if (CurrentPawnOnScene)
	{
		Possess(Cast<APawn>(CurrentPawnOnScene));
	}
}