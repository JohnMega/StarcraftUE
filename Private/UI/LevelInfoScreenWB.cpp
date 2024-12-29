// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LevelInfoScreenWB.h"
#include "Components/Button.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"
#include "MediaPlayer.h"
#include "FileMediaSource.h"

bool ULevelInfoScreenWB::Initialize()
{
	bool InitRes = Super::Initialize();

	if (InfoButton)
		InfoButton->OnClicked.AddDynamic(this, &ULevelInfoScreenWB::OnInfoButtonClicked);

	if (ContinueButton)
		ContinueButton->OnClicked.AddDynamic(this, &ULevelInfoScreenWB::OnContinueButtonClicked);

	return InitRes;
}

void ULevelInfoScreenWB::OnInfoButtonClicked()
{
	InfoButton->SetIsEnabled(false);
	PlayAnimation(MediaShowAnim);
	MinionRepairMediaPlayer->OpenSource(MinionRepairMediaSource);
}

void ULevelInfoScreenWB::OnContinueButtonClicked()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	SetVisibility(ESlateVisibility::Hidden);
}