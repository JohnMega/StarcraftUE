// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/MenuUnitWB.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/WidgetAnimation.h"
#include "UI/HUD/SCMenuHUD.h"
#include "UI/Menu/RaynorDescription/StimpackDescriptionWB.h"

bool UMenuUnitWB::Initialize()
{
	bool InitRes = Super::Initialize();

	if (RaynorUnitButton)
	{
		RaynorUnitButton->OnClicked.AddDynamic(this, &UMenuUnitWB::OnRaynorUnitButtonClicked);
		RaynorUnitButton->OnHovered.AddDynamic(this, &UMenuUnitWB::OnRaynorUnitButtonHovered);
		RaynorUnitButton->OnUnhovered.AddDynamic(this, &UMenuUnitWB::OnRaynorUnitButtonUnhovered);

		if (GetWorld()->GetAuthGameMode())
		{
			OnRaynorUnitButtonClicked();
		}
	}

	if (MinionUnitButton)
	{
		MinionUnitButton->OnClicked.AddDynamic(this, &UMenuUnitWB::OnMinionUnitButtonClicked);
		MinionUnitButton->OnHovered.AddDynamic(this, &UMenuUnitWB::OnMinionUnitButtonHovered);
		MinionUnitButton->OnUnhovered.AddDynamic(this, &UMenuUnitWB::OnMinionUnitButtonUnhovered);
	}

	AllButtons.Add(RaynorUnitButton);
	AllButtons.Add(MinionUnitButton);

	AllPressedAnimations.Add(RaynorPressedAnim);
	AllPressedAnimations.Add(MinionPressedAnim);

	return InitRes;
}

void UMenuUnitWB::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	if (InVisibility == ESlateVisibility::Visible)
	{
		OnRaynorUnitButtonClicked();
	}
	else
	{
		RaynorDescriptionWB->SetVisibility(InVisibility);
		MinionDescriptionWB->SetVisibility(InVisibility);
	}
}

void UMenuUnitWB::SetEnableForAllButtons(bool IsEnable)
{
	for (int32 i = 0; i < AllButtons.Num(); ++i)
	{
		if (!AllButtons[i]->bIsEnabled) PlayAnimationReverse(AllPressedAnimations[i]);
		AllButtons[i]->SetIsEnabled(IsEnable);
	}
}

void UMenuUnitWB::SetActorHiddenInGameForAllUnits(bool IsVisible)
{
	for (int32 i = 0; i < AllTalkUnitsOnSceneClasses.Num(); ++i)
	{
		auto TalkUnit = UGameplayStatics::GetActorOfClass(GetWorld(), AllTalkUnitsOnSceneClasses[i].Get());
		TalkUnit->GetRootComponent()->SetVisibility(IsVisible, true);
	}
}

void UMenuUnitWB::SetVisibilityForAllWB(bool IsVisible)
{
	RaynorDescriptionWB->SetVisibility(IsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	MinionDescriptionWB->SetVisibility(IsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UMenuUnitWB::OnRaynorUnitButtonClicked()
{
	SetEnableForAllButtons(true);
	SetVisibilityForAllWB(false);
	RaynorDescriptionWB->SetVisibility(ESlateVisibility::Visible);

	PlayAnimation(RaynorPressedAnim);
	RaynorUnitButton->SetIsEnabled(false);

	SetActorHiddenInGameForAllUnits(false);
	auto RaynorTalk = UGameplayStatics::GetActorOfClass(GetWorld(), AllTalkUnitsOnSceneClasses[TalkRaynorIndex].Get());
	RaynorTalk->GetRootComponent()->SetVisibility(true, true);
}

void UMenuUnitWB::OnRaynorUnitButtonHovered()
{
	PlayAnimation(RaynorHoveredAnim);
}

void UMenuUnitWB::OnRaynorUnitButtonUnhovered()
{
	PlayAnimationReverse(RaynorHoveredAnim);
}

void UMenuUnitWB::OnMinionUnitButtonClicked()
{
	SetEnableForAllButtons(true);
	SetVisibilityForAllWB(false);
	MinionDescriptionWB->SetVisibility(ESlateVisibility::Visible);

	PlayAnimation(MinionPressedAnim);
	MinionUnitButton->SetIsEnabled(false);

	SetActorHiddenInGameForAllUnits(false);
	auto MinionTalk = UGameplayStatics::GetActorOfClass(GetWorld(), AllTalkUnitsOnSceneClasses[TalkMinionIndex].Get());
	MinionTalk->GetRootComponent()->SetVisibility(true, true);
}

void UMenuUnitWB::OnMinionUnitButtonHovered()
{
	PlayAnimation(MinionHoveredAnim);
}

void UMenuUnitWB::OnMinionUnitButtonUnhovered()
{
	PlayAnimationReverse(MinionHoveredAnim);
}