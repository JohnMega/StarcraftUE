// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimerWB.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimerFinishSignature);

class UTextBlock;

UCLASS()
class STARCRAFT_API UTimerWB : public UUserWidget
{
	GENERATED_BODY()
	
private:
	FTimerHandle TimerHandle;

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimerText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Time")
	int32 SecondsCount = 60;

public:
	UPROPERTY(BlueprintAssignable)
	FOnTimerFinishSignature OnTimerFinish;

private:
	UFUNCTION()
	void SetTimerText();

public:
	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable)
	void StopTimer();
};
