// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/GeneralUITypes.h"
#include "RaynorDescriptionWB.generated.h"

class UButton;
class UStimpackDescriptionWB;
class UMarineHealthKitDescriptionWB;

DECLARE_MULTICAST_DELEGATE(FOnStimpackPressedSignature)
DECLARE_MULTICAST_DELEGATE(FOnMarineHealthKitPressedSignature)

UCLASS()
class STARCRAFT_API URaynorDescriptionWB : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UStimpackDescriptionWB* StimpackDescriptionWB;
	UMarineHealthKitDescriptionWB* MarineHealthKitDescriptionWB;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* StimpackButton;

	UPROPERTY(meta = (BindWidget))
	UButton* HealthKitButton;

public:
	FOnStimpackPressedSignature OnStimpackPressed;
	FOnMarineHealthKitPressedSignature OnMarineHealthKitPressed;

private:
	UFUNCTION()
	void OnStimpackButtonPressed();

	UFUNCTION()
	void OnHealthKitButtonPressed();

	void HiddenAllDescriptionWidgets();

public:
	virtual bool Initialize() override;

	virtual void SetVisibility(ESlateVisibility InVisibility) override;
};
