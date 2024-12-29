// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UnitTestCameraWidgetComponent.h"
#include "UI/InGameMenuWB.h"

void UUnitTestCameraWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	InGameMenuWidget = CreateWidget<UInGameMenuWB>(GetWorld(), InGameMenuClass.Get());
	if (InGameMenuWidget)
	{
		CreatedWidgets.Add(InGameMenuWidget);
		InGameMenuWidget->AddToViewport();
	}

	InGameMenuWidget->BeginPlay();
}