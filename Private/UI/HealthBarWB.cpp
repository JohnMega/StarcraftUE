// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HealthBarWB.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/HealthComponent.h"
#include "Components/ProgressBar.h"

bool UHealthBarWB::Initialize()
{
	bool InitRes = Super::Initialize();
	
	return InitRes;
}

void UHealthBarWB::SetHealthPercent(float HealthPercent)
{
	HealthProgressBar->SetPercent(HealthPercent);
	FLinearColor Green = { 0, 1, 0 };
	FLinearColor Yellow = { 1, 1, 0 };
	FLinearColor Red = { 1, 0, 0 };
	if (HealthPercent >= 0.5f && HealthPercent <= 1)
	{
		HealthProgressBar->SetFillColorAndOpacity(FMath::Lerp(Yellow, Green, 2 * HealthPercent - 1));
	}
	else if (HealthPercent >= 0.25f && HealthPercent <= 0.5f)
	{
		HealthProgressBar->SetFillColorAndOpacity(FMath::Lerp(Red, Yellow, 4 * HealthPercent - 1));
	}
}