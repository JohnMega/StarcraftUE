// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/UserSettingsMenu/SettingWB.h"
#include "UI/Menu/UserSettingsMenu/SettingButtonWB.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/GameUserSettings.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"

bool USettingWB::Initialize()
{
	bool InitRes = Super::Initialize();

	if (UGameplayStatics::GetGameInstance(GetWorld()))
		GameUserSettings = UGameplayStatics::GetGameInstance(GetWorld())->GetEngine()->GetGameUserSettings();

	if (SettingNameText && GameUserSettings)
	{
		SettingNameText->SetText(FText::FromString(SettingName.ToString()));
	}

	if (SettingHoveredButton)
	{
		SettingHoveredButton->OnHovered.AddDynamic(this, &USettingWB::OnSettingHoveredButtonHovered);
		SettingHoveredButton->OnUnhovered.AddDynamic(this, &USettingWB::OnSettingHoveredButtonUnhovered);
	}
	
	if (LowSettingWB) LowSettingWB->SetOwnerWidget(this);
	if (MediumSettingWB) MediumSettingWB->SetOwnerWidget(this);
	if (HighSettingWB) HighSettingWB->SetOwnerWidget(this);
	if (EpicSettingWB) EpicSettingWB->SetOwnerWidget(this);
	if (UltraSettingWB) UltraSettingWB->SetOwnerWidget(this);

	SettingButtonsWidgets.Add(LowSettingWB);
	SettingButtonsWidgets.Add(MediumSettingWB);
	SettingButtonsWidgets.Add(HighSettingWB);
	SettingButtonsWidgets.Add(EpicSettingWB);
	SettingButtonsWidgets.Add(UltraSettingWB);

	return InitRes;
}

FName USettingWB::GetGUSFuncGetterName() const
{
	return GUSFuncGetterName;
}

FName USettingWB::GetGUSFuncSetterName() const
{
	return GUSFuncSetterName;
}

int32 USettingWB::CallGUSFuncGetter() const
{
	auto Func = GameUserSettings->FindFunction(GUSFuncGetterName);
	
	FFrame FuncFFrame(GameUserSettings, Func, nullptr);
	int32 CurrentSettingValue;
	GameUserSettings->CallFunction(FuncFFrame, &CurrentSettingValue, Func);

	return CurrentSettingValue;
}

void USettingWB::CallGUSFuncSetter(int32 SettingValue)
{
	auto Func = GameUserSettings->FindFunction(GUSFuncSetterName);
	
	NewSettingValue = SettingValue;
	FFrame FuncFFrame(GameUserSettings, Func, this, (FFrame*)0, GetClass()->FindPropertyByName("NewSettingValue"));
	GameUserSettings->CallFunction(FuncFFrame, nullptr, Func);
}

void USettingWB::UpdateSettingText() const
{
	int32 GUSFuncGetterResult = CallGUSFuncGetter();
	for (int32 i = 0; i < SettingButtonsWidgets.Num(); ++i)
	{
		SettingButtonsWidgets[i]->SetSettingButtonSelected(GUSFuncGetterResult == SettingButtonsWidgets[i]->GetSettingValue());
	}
}

void USettingWB::OnSettingButtonClicked()
{
	GameUserSettings->ApplySettings(true);
	
	TArray<UUserWidget*> AllSettings;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), AllSettings, GetClass(), false);
	for (int32 i = 0; i < AllSettings.Num(); ++i)
	{
		Cast<USettingWB>(AllSettings[i])->UpdateSettingText();
	}
}

void USettingWB::OnSettingHoveredButtonHovered()
{
	PlayAnimation(SettingHoveredAnim);
}

void USettingWB::OnSettingHoveredButtonUnhovered()
{
	PlayAnimationReverse(SettingHoveredAnim);
}