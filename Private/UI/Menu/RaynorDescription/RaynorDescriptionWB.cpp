// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/RaynorDescription/RaynorDescriptionWB.h"
#include "UI/Menu/RaynorDescription/StimpackDescriptionWB.h"
#include "UI/Menu/RaynorDescription/MarineHealthKitDescriptionWB.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/SCMenuHUD.h"

bool URaynorDescriptionWB::Initialize()
{
	bool InitRes = Super::Initialize();

	if (StimpackButton)
	{
		StimpackButton->OnPressed.AddDynamic(this, &URaynorDescriptionWB::OnStimpackButtonPressed);
	}

	if (HealthKitButton)
	{
		HealthKitButton->OnPressed.AddDynamic(this, &URaynorDescriptionWB::OnHealthKitButtonPressed);
	}

	if (GetWorld()->GetAuthGameMode())
	{
		auto SCMenuHUD = Cast<ASCMenuHUD>((*GetWorld()->GetPlayerControllerIterator())->GetHUD());
		StimpackDescriptionWB = SCMenuHUD->GetWidget<UStimpackDescriptionWB>();
		if (StimpackDescriptionWB)
		{
			OnStimpackPressed.AddUObject(StimpackDescriptionWB, &UStimpackDescriptionWB::OnStimpackPressed);
		}

		MarineHealthKitDescriptionWB = SCMenuHUD->GetWidget<UMarineHealthKitDescriptionWB>();
		if (MarineHealthKitDescriptionWB)
		{
			OnMarineHealthKitPressed.AddUObject(MarineHealthKitDescriptionWB, &UMarineHealthKitDescriptionWB::OnMarineHealthKitPressed);
		}
	}

	return InitRes;
}

void URaynorDescriptionWB::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	StimpackDescriptionWB->SetVisibility(InVisibility == ESlateVisibility::Visible ? ESlateVisibility::HitTestInvisible : InVisibility);
	MarineHealthKitDescriptionWB->SetVisibility(InVisibility == ESlateVisibility::Visible ? ESlateVisibility::HitTestInvisible : InVisibility);
}

void URaynorDescriptionWB::HiddenAllDescriptionWidgets()
{
	StimpackDescriptionWB->SetVisibility(ESlateVisibility::Hidden);
	MarineHealthKitDescriptionWB->SetVisibility(ESlateVisibility::Hidden);
}

void URaynorDescriptionWB::OnStimpackButtonPressed()
{
	//HiddenAllDescriptionWidgets();
	OnStimpackPressed.Broadcast();
}

void URaynorDescriptionWB::OnHealthKitButtonPressed()
{
	//HiddenAllDescriptionWidgets();
	OnMarineHealthKitPressed.Broadcast();
}