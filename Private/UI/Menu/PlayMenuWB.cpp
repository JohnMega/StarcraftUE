// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/PlayMenuWB.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

bool UPlayMenuWB::Initialize()
{
	bool InitRes = Super::Initialize();

	if (DemoLevelButton)
	{
		DemoLevelButton->OnClicked.AddDynamic(this, &UPlayMenuWB::OnDemoLevelButtonClicked);
		DemoLevelButton->SetIsEnabled(false);
	}

	if (UnitTestLevelButton)
		UnitTestLevelButton->OnClicked.AddDynamic(this, &UPlayMenuWB::OnUnitTestLevelButtonButtonClicked);

	if (DemoLevelButton && UnitTestLevelButton)
	{
		DemoLevelPlayMenuWB->SetVisibility(ESlateVisibility::Visible);
		UnitTestLevelPlayMenuWB->SetVisibility(ESlateVisibility::Hidden);
	}

	return InitRes;
}

void UPlayMenuWB::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	if (InVisibility == ESlateVisibility::Hidden)
	{
		DemoLevelPlayMenuWB->SetVisibility(ESlateVisibility::Hidden);
		UnitTestLevelPlayMenuWB->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		OnDemoLevelButtonClicked();
	}
}

void UPlayMenuWB::OnDemoLevelButtonClicked()
{
	DemoLevelPlayMenuWB->SetVisibility(ESlateVisibility::Visible);
	UnitTestLevelPlayMenuWB->SetVisibility(ESlateVisibility::Hidden);

	DemoLevelButton->SetIsEnabled(false);
	UnitTestLevelButton->SetIsEnabled(true);
}

void UPlayMenuWB::OnUnitTestLevelButtonButtonClicked()
{
	DemoLevelPlayMenuWB->SetVisibility(ESlateVisibility::Hidden);
	UnitTestLevelPlayMenuWB->SetVisibility(ESlateVisibility::Visible);

	DemoLevelButton->SetIsEnabled(true);
	UnitTestLevelButton->SetIsEnabled(false);
}