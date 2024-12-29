// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/PlayMenu/UnitTestLevelPlayMenuWB.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "MediaPlayer.h"
#include "FileMediaSource.h"

bool UUnitTestLevelPlayMenuWB::Initialize()
{
	bool InitRes = Super::Initialize();

	if (PlayButton)
		PlayButton->OnClicked.AddDynamic(this, &UUnitTestLevelPlayMenuWB::OnPlayButtonClicked);

	if (GetWorld()->GetAuthGameMode())
		UnitTestMediaPlayer->OpenSource(UnitTestMediaSource);

	return InitRes;
}

void UUnitTestLevelPlayMenuWB::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	if(InVisibility == ESlateVisibility::Visible)
		UnitTestMediaPlayer->OpenSource(UnitTestMediaSource);
	else if (InVisibility == ESlateVisibility::Hidden)
		UnitTestMediaPlayer->Close();
}

void UUnitTestLevelPlayMenuWB::OnPlayButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), UnitTestLevelName);
}