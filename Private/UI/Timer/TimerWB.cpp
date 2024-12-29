// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Timer/TimerWB.h"
#include "Components/TextBlock.h"

bool UTimerWB::Initialize()
{
	bool InitRes = Super::Initialize();

	if (GetGameInstance())
	{
		SetTimerText();
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTimerWB::SetTimerText, 1.0f, true);
	}

	return InitRes;
}

void UTimerWB::StopTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UTimerWB::SetTimerText()
{
	if (SecondsCount <= 0) 
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		OnTimerFinish.Broadcast();
		return;
	}

	--SecondsCount;

	int32 MinutesCount = SecondsCount / 60;
	
	FString MinutesText = MinutesCount < 10 ? FString("0") + FString::FromInt(MinutesCount) : FString::FromInt(MinutesCount);
	FString SecondsText = SecondsCount - MinutesCount * 60 < 10 ? FString("0") + FString::FromInt(SecondsCount - MinutesCount * 60) : FString::FromInt(SecondsCount - MinutesCount * 60);
	TimerText->SetText(FText::FromString(MinutesText + FString(" : ") + SecondsText));
}