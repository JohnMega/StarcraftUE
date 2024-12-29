// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MainCameraWidgetComponent.h"
#include "GameFramework/GameModeBase.h"
#include "UI/SelectBoxWB.h"
#include "UI/InGameMenuWB.h"
#include "UI/UnitSkillsWB.h"
#include "UI/UnitTalk/UnitTalkWB.h"
#include "UI/Minimap/MinimapWB.h"
#include "UI/GameMenu/GameMenuWB.h"

UMainCameraWidgetComponent::UMainCameraWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMainCameraWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	SelectBoxWidget = CreateWidget<USelectBoxWB>(GetWorld(), SelectBoxClass.Get());
	if (SelectBoxWidget)
	{
		CreatedWidgets.Add(SelectBoxWidget);
		SelectBoxWidget->AddToViewport();
	}

	UnitSkillsWidget = CreateWidget<UUnitSkillsWB>(GetWorld(), UnitSkillsClass.Get());
	if (UnitSkillsWidget)
	{
		CreatedWidgets.Add(UnitSkillsWidget);
		UnitSkillsWidget->AddToViewport();
	}

	UnitTalkWidget = CreateWidget<UUnitTalkWB>(GetWorld(), UnitTalkClass.Get());
	if (UnitTalkWidget)
	{
		CreatedWidgets.Add(UnitTalkWidget);
		UnitTalkWidget->AddToViewport();
	}

	MinimapWidget = CreateWidget<UMinimapWB>(GetWorld(), MinimapClass.Get());
	if (MinimapWidget)
	{
		CreatedWidgets.Add(MinimapWidget);
		MinimapWidget->AddToViewport();
	}

	GameMenuWidget = CreateWidget<UGameMenuWB>(GetWorld(), GameMenuClass.Get());
	if (GameMenuWidget)
	{
		CreatedWidgets.Add(GameMenuWidget);
		GameMenuWidget->AddToViewport();
	}

	UnitTalkWidget->BeginPlay();
	UnitSkillsWidget->BeginPlay();
	GameMenuWidget->BeginPlay();
}

void UMainCameraWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SelectBoxWidget->SelectBoxTick();
}