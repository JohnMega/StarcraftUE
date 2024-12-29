// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TaskWindowWB.generated.h"

class UTextBlock;

UCLASS()
class STARCRAFT_API UTaskWindowWB : public UUserWidget
{
	GENERATED_BODY()
	
private:
	FString TempTaskTextStr;
	FName CurrentTaskText;
	int32 CurrentTaskTextPtr;
	FTimerHandle SetTaskTextHandleTimer;

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TaskText;

private:
	void SetTaskTextHandle();

public:
	UFUNCTION(BlueprintCallable)
	void SetTaskText(FName NewTaskText);
};
