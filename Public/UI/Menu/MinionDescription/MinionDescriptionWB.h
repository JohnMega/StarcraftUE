// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MinionDescriptionWB.generated.h"

class UButton;
class UMinionHealthKitDescriptionWB;

DECLARE_MULTICAST_DELEGATE(FOnMinionHealthKitPressedSignature)

UCLASS()
class STARCRAFT_API UMinionDescriptionWB : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UMinionHealthKitDescriptionWB* MinionHealthKitDescriptionWB;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* HealthKitButton;

public:
	FOnMinionHealthKitPressedSignature OnMinionHealthKitPressed;

private:
	UFUNCTION()
	void OnHealthKitButtonPressed();

	void HiddenAllDescriptionWidgets();

public:
	virtual bool Initialize() override;

	virtual void SetVisibility(ESlateVisibility InVisibility) override;
};
