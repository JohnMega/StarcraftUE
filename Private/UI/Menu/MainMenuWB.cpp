// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/MainMenuWB.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Materials/MaterialInstance.h"
#include "Styling/SlateTypes.h"
#include "Styling/SlateBrush.h"
#include "UI/Menu/MenuUnitWB.h"
#include "UI/Menu/PlayMenuWB.h"
#include "UI/Menu/UserSettingsWB.h"
#include "Animation/WidgetAnimation.h"

bool UMainMenuWB::Initialize()
{
	bool InitRes = Super::Initialize();

	if (PlaySectionButton)
	{
		PlaySectionButton->OnPressed.AddDynamic(this, &UMainMenuWB::OnPlaySectionButtonPressed);
		PlaySectionButton->OnHovered.AddDynamic(this, &UMainMenuWB::OnPlaySectionButtonHovered);
		PlaySectionButton->OnUnhovered.AddDynamic(this, &UMainMenuWB::OnPlaySectionButtonUnhovered);

		PlaySectionButton->SetIsEnabled(false);
		MenuPlayWB->SetVisibility(ESlateVisibility::Visible);
	}

	if (UnitsSectionButton)
	{
		UnitsSectionButton->OnPressed.AddDynamic(this, &UMainMenuWB::OnUnitsSectionButtonPressed);
		UnitsSectionButton->OnHovered.AddDynamic(this, &UMainMenuWB::OnUnitsSectionButtonHovered);
		UnitsSectionButton->OnUnhovered.AddDynamic(this, &UMainMenuWB::OnUnitsSectionButtonUnhovered);
	}

	if (SettingsSectionButton)
	{
		SettingsSectionButton->OnPressed.AddDynamic(this, &UMainMenuWB::OnSettingsSectionButtonPressed);
		SettingsSectionButton->OnHovered.AddDynamic(this, &UMainMenuWB::OnSettingsSectionButtonHovered);
		SettingsSectionButton->OnUnhovered.AddDynamic(this, &UMainMenuWB::OnSettingsSectionButtonUnhovered);
	}

	if (ExitSectionButton)
	{
		ExitSectionButton->OnPressed.AddDynamic(this, &UMainMenuWB::OnExitSectionButtonPressed);
		ExitSectionButton->OnHovered.AddDynamic(this, &UMainMenuWB::OnExitSectionButtonHovered);
		ExitSectionButton->OnUnhovered.AddDynamic(this, &UMainMenuWB::OnExitSectionButtonUnhovered);
	}
	
	AllButtons.Add(PlaySectionButton);
	AllButtons.Add(UnitsSectionButton);
	AllButtons.Add(SettingsSectionButton);
	AllButtons.Add(ExitSectionButton);

	AllPressedAnimations.Add(PlayPressedAnim);
	AllPressedAnimations.Add(UnitsPressedAnim);
	AllPressedAnimations.Add(SettingsPressedAnim);
	AllPressedAnimations.Add(ExitPressedAnim);

	AllSubUserWidgets.Add(MenuPlayWB);
	AllSubUserWidgets.Add(MenuUnitWB);
	AllSubUserWidgets.Add(UserSettingsWB);

	return InitRes;
}

void UMainMenuWB::SetEnableForAllButtons(bool IsEnable)
{
	for (int32 i = 0; i < AllButtons.Num(); ++i)
	{
		if (!AllButtons[i]->bIsEnabled) PlayAnimationReverse(AllPressedAnimations[i]);
		AllButtons[i]->SetIsEnabled(IsEnable);
	}
}

void UMainMenuWB::SetVisibilityForAllWB(bool IsVisible)
{
	ESlateVisibility WBVisibility = IsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden;

	for (int32 i = 0; i < AllSubUserWidgets.Num(); ++i)
		AllSubUserWidgets[i]->SetVisibility(WBVisibility);
}

void UMainMenuWB::OnPlaySectionButtonPressed()
{
	SetEnableForAllButtons(true);
	SetVisibilityForAllWB(false);
	
	MenuPlayWB->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(PlayPressedAnim);
	PlaySectionButton->SetIsEnabled(false);
}

void UMainMenuWB::OnPlaySectionButtonHovered()
{
	PlayAnimation(PlayHoveredAnim);
}

void UMainMenuWB::OnPlaySectionButtonUnhovered()
{
	PlayAnimationReverse(PlayHoveredAnim);
}

void UMainMenuWB::OnUnitsSectionButtonPressed()
{
	SetEnableForAllButtons(true);
	SetVisibilityForAllWB(false);

	MenuUnitWB->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(UnitsPressedAnim);
	UnitsSectionButton->SetIsEnabled(false);
}

void UMainMenuWB::OnUnitsSectionButtonHovered()
{
	PlayAnimation(UnitsHoveredAnim);
}

void UMainMenuWB::OnUnitsSectionButtonUnhovered()
{
	PlayAnimationReverse(UnitsHoveredAnim);
}

void UMainMenuWB::OnSettingsSectionButtonPressed()
{
	SetEnableForAllButtons(true);
	SetVisibilityForAllWB(false);

	UserSettingsWB->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(SettingsPressedAnim);
	SettingsSectionButton->SetIsEnabled(false);
}

void UMainMenuWB::OnSettingsSectionButtonHovered()
{
	PlayAnimation(SettingsHoveredAnim);
}

void UMainMenuWB::OnSettingsSectionButtonUnhovered()
{
	PlayAnimationReverse(SettingsHoveredAnim);
}

void UMainMenuWB::OnExitSectionButtonPressed()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, true);
}

void UMainMenuWB::OnExitSectionButtonHovered()
{
	PlayAnimation(ExitHoveredAnim);
}

void UMainMenuWB::OnExitSectionButtonUnhovered()
{
	PlayAnimationReverse(ExitHoveredAnim);
}