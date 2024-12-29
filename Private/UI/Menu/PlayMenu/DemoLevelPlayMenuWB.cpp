// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/PlayMenu/DemoLevelPlayMenuWB.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "MediaPlayer.h"
#include "FileMediaSource.h"

bool UDemoLevelPlayMenuWB::Initialize()
{
	bool InitRes = Super::Initialize();

	if (PlayButton)
		PlayButton->OnClicked.AddDynamic(this, &UDemoLevelPlayMenuWB::OnPlayButtonClicked);

	if (GetWorld()->GetAuthGameMode())
		DemoLevelMediaPlayer->OpenSource(DemoLevelMediaSource);

	return InitRes;
}

void UDemoLevelPlayMenuWB::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	if(InVisibility == ESlateVisibility::Visible)
		DemoLevelMediaPlayer->OpenSource(DemoLevelMediaSource);
	else if (InVisibility == ESlateVisibility::Hidden)
		DemoLevelMediaPlayer->Close();
}

void UDemoLevelPlayMenuWB::OnPlayButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), DemoLevelName);
}