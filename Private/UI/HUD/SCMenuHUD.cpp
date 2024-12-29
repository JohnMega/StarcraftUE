// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/SCMenuHUD.h"
#include "UI/Menu/MainMenuWB.h"
#include "UI/Menu/MenuUnitWB.h"
#include "UI/Menu/RaynorDescription/StimpackDescriptionWB.h"
#include "UI/Menu/RaynorDescription/MarineHealthKitDescriptionWB.h"
#include "UI/Menu/MinionDescription/MinionHealthKitDescriptionWB.h"

void ASCMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	auto StimpackDescriptionWB = CreateWidget(GetWorld(), StimpackDescriptionWBClass.Get());
	if (StimpackDescriptionWB)
	{
		CreatedWidgets.Add(StimpackDescriptionWB);
	}

	auto MarineHealthKitDescriptionWB = CreateWidget(GetWorld(), MarineHealthKitDescriptionWBClass.Get());
	if (MarineHealthKitDescriptionWB)
	{
		CreatedWidgets.Add(MarineHealthKitDescriptionWB);
	}

	auto MinionHealthKitDescriptionWB = CreateWidget(GetWorld(), MinionHealthKitDescriptionWBClass.Get());
	if (MinionHealthKitDescriptionWB)
	{
		CreatedWidgets.Add(MinionHealthKitDescriptionWB);
	}

	auto MainMenuWB = CreateWidget(GetWorld(), MainMenuWBClass.Get());
	if (MainMenuWB)
	{
		CreatedWidgets.Add(MainMenuWB);
	}

	MainMenuWB->AddToViewport();
	StimpackDescriptionWB->AddToViewport();
	MarineHealthKitDescriptionWB->AddToViewport();
	MinionHealthKitDescriptionWB->AddToViewport();
}