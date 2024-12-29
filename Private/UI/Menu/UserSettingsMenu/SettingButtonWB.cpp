// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/UserSettingsMenu/SettingButtonWB.h"
#include "UI/Menu/UserSettingsMenu/SettingWB.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"

bool USettingButtonWB::Initialize()
{
	bool InitRes = Super::Initialize();

	if (SettingButton)
	{
		SettingButton->OnPressed.AddDynamic(this, &USettingButtonWB::OnSettingButtonPressed);
		SettingButton->OnHovered.AddDynamic(this, &USettingButtonWB::OnSettingButtonHovered);
		SettingButton->OnUnhovered.AddDynamic(this, &USettingButtonWB::OnSettingButtonUnhovered);
	}
	
	if (SettingText)
	{
		SettingText->SetText(FText::FromString(SettingTextProperty.ToString()));
	}

	return InitRes;
}

void USettingButtonWB::SetOwnerWidget(USettingWB* NewOwner)
{
	Owner = NewOwner;

	if (UGameplayStatics::GetGameInstance(GetWorld()) && Owner->CallGUSFuncGetter() == SettingValue)
		SetSettingButtonSelected(true);
}

bool USettingButtonWB::IsSettingButtonSelected() const
{
	return bIsSelected;
}

void USettingButtonWB::SetSettingButtonSelected(bool NewIsSelected)
{
	if (NewIsSelected != bIsSelected)
	{
		NewIsSelected ? PlayAnimation(SettingPressedAnim) : PlayAnimationReverse(SettingPressedAnim);
	}

	bIsSelected = NewIsSelected;
}

int32 USettingButtonWB::GetSettingValue() const
{
	return SettingValue;
}

void USettingButtonWB::OnSettingButtonPressed()
{
	Owner->CallGUSFuncSetter(SettingValue);
	Owner->OnSettingButtonClicked();
}

void USettingButtonWB::OnSettingButtonHovered()
{
	PlayAnimation(SettingHoveredAnim);
}

void USettingButtonWB::OnSettingButtonUnhovered()
{
	PlayAnimationReverse(SettingHoveredAnim);
}