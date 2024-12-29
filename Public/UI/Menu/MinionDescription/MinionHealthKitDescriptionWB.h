// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MinionHealthKitDescriptionWB.generated.h"

class UMediaPlayer;
class UMediaSource;
class UWidgetAnimation;

UCLASS()
class STARCRAFT_API UMinionHealthKitDescriptionWB : public UUserWidget
{
	GENERATED_BODY()
	
private:
	bool IsHealthKitButtonPressed = false;

protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* HealthKitDescriptionVisibilityAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Media")
	UMediaPlayer* HealthKitMediaPlayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Media")
	UMediaSource* HealthKitMediaSource;

public:
	virtual void SetVisibility(ESlateVisibility InVisibility) override;

	void OnMinionHealthKitPressed();
};
