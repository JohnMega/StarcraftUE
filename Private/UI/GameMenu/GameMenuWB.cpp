// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameMenu/GameMenuWB.h"
#include "UI/Menu/UserSettingsWB.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/Overlay.h"

bool UGameMenuWB::Initialize()
{
	bool InitRes = Super::Initialize();

	if (ContinueButton)
		ContinueButton->OnClicked.AddDynamic(this, &UGameMenuWB::OnContinueButtonClicked);

	if (SettingsButton)
		SettingsButton->OnClicked.AddDynamic(this, &UGameMenuWB::OnSettingsButtonClicked);

	if (ExitButton)
		ExitButton->OnClicked.AddDynamic(this, &UGameMenuWB::OnExitButtonClicked);

	if(SettingsBackButton)
		SettingsBackButton->OnClicked.AddDynamic(this, &UGameMenuWB::OnSettingsBackButtonClicked);

	return InitRes;
}

void UGameMenuWB::BeginPlay()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UGameMenuWB::OnGameMenuEnable()
{
	if (UGameplayStatics::IsGamePaused(GetWorld())) return;

	UGameplayStatics::SetGamePaused(GetWorld(), true);
	SetVisibility(ESlateVisibility::Visible);
}

void UGameMenuWB::OnContinueButtonClicked()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	SetVisibility(ESlateVisibility::Hidden);
}

void UGameMenuWB::OnSettingsButtonClicked()
{
	GameMenuOverlay->SetVisibility(ESlateVisibility::Hidden);
	SettingsBackOverlay->SetVisibility(ESlateVisibility::Visible);

	UserSettingsWB->SetVisibility(ESlateVisibility::Visible);
	UserSettingsWB->SetColorAndOpacity({ 1,1,1,1 });
}

void UGameMenuWB::OnExitButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), ManuLevelName);
}

void UGameMenuWB::OnSettingsBackButtonClicked()
{
	GameMenuOverlay->SetVisibility(ESlateVisibility::Visible);
	SettingsBackOverlay->SetVisibility(ESlateVisibility::Hidden);

	UserSettingsWB->SetVisibility(ESlateVisibility::Hidden);
	UserSettingsWB->SetColorAndOpacity({ 1,1,1,0 });
}