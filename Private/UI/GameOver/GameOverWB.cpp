// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameOver/GameOverWB.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

bool UGameOverWB::Initialize()
{
	bool InitRes = Super::Initialize();

	if (RetryButton)
		RetryButton->OnClicked.AddDynamic(this, &UGameOverWB::OnRetryButtonClicked);

	if (BackToMenuButton)
		BackToMenuButton->OnClicked.AddDynamic(this, &UGameOverWB::OnBackToMenuButtonClicked);

	return InitRes;
}

void UGameOverWB::OnRetryButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), ThisLevelName);
}

void UGameOverWB::OnBackToMenuButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), MenuLevelName);
}