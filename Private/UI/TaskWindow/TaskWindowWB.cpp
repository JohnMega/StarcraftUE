// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TaskWindow/TaskWindowWB.h"
#include "Components/TextBlock.h"

void UTaskWindowWB::SetTaskText(FName NewTaskText)
{
	CurrentTaskTextPtr = 0;
	CurrentTaskText = NewTaskText;
	TempTaskTextStr.Empty();
	GetWorld()->GetTimerManager().SetTimer(SetTaskTextHandleTimer, this, &UTaskWindowWB::SetTaskTextHandle, 0.1f, true);
}

void UTaskWindowWB::SetTaskTextHandle()
{
	TempTaskTextStr.AppendChar(CurrentTaskText.ToString()[CurrentTaskTextPtr++]);
	TaskText->SetText(FText::FromString(TempTaskTextStr));

	if (CurrentTaskTextPtr == CurrentTaskText.GetStringLength())
	{
		GetWorld()->GetTimerManager().ClearTimer(SetTaskTextHandleTimer);
	}
}