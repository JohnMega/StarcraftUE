// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Characters/Minion/MinionRepairKeyButton.h"
#include "AI/Characters/Minion/SCMinion.h"
#include "Components/WidgetComponent.h"

// Sets default values
AMinionRepairKeyButton::AMinionRepairKeyButton()
{
	PrimaryActorTick.bCanEverTick = true;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Widget Component");
	WidgetComponent->SetupAttachment(GetRootComponent());
}

void AMinionRepairKeyButton::BeginPlay()
{
	Super::BeginPlay();
}

void AMinionRepairKeyButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AMinionRepairKeyButton::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);

	if (NewOwner)
	{
		Cast<ASCMinion>(NewOwner)->OnRepairKeyButtonComponentVisibility.AddUObject(this, &AMinionRepairKeyButton::OnRepairKeyButtonComponentVisibility);
		Cast<ASCMinion>(NewOwner)->OnMinionEnableState.AddUObject(this, &AMinionRepairKeyButton::OnMinionEnableState);
	}
}

void AMinionRepairKeyButton::OnRepairKeyButtonComponentVisibility(ESlateVisibility InVisibility)
{
	WidgetComponent->GetUserWidgetObject()->SetVisibility(InVisibility);
}

void AMinionRepairKeyButton::OnMinionEnableState()
{
	WidgetComponent->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Hidden);
}