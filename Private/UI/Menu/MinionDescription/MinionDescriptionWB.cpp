// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/MinionDescription/MinionDescriptionWB.h"
#include "UI/Menu/MinionDescription/MinionHealthKitDescriptionWB.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/SCMenuHUD.h"

bool UMinionDescriptionWB::Initialize()
{
	bool InitRes = Super::Initialize();

	if (HealthKitButton)
	{
		HealthKitButton->OnPressed.AddDynamic(this, &UMinionDescriptionWB::OnHealthKitButtonPressed);
	}

	if (GetWorld()->GetAuthGameMode())
	{
		auto SCMenuHUD = Cast<ASCMenuHUD>((*GetWorld()->GetPlayerControllerIterator())->GetHUD());
		MinionHealthKitDescriptionWB = SCMenuHUD->GetWidget<UMinionHealthKitDescriptionWB>();
		if (MinionHealthKitDescriptionWB)
		{
			OnMinionHealthKitPressed.AddUObject(MinionHealthKitDescriptionWB, &UMinionHealthKitDescriptionWB::OnMinionHealthKitPressed);
		}
	}

	return InitRes;
}

void UMinionDescriptionWB::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	MinionHealthKitDescriptionWB->SetVisibility(InVisibility == ESlateVisibility::Visible ? ESlateVisibility::HitTestInvisible : InVisibility);
}

void UMinionDescriptionWB::HiddenAllDescriptionWidgets()
{
	MinionHealthKitDescriptionWB->SetVisibility(ESlateVisibility::Hidden);
}

void UMinionDescriptionWB::OnHealthKitButtonPressed()
{
	OnMinionHealthKitPressed.Broadcast();
}
