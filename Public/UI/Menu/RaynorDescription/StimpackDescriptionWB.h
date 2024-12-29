// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StimpackDescriptionWB.generated.h"

class UMediaPlayer;
class UMediaSource;
class UWidgetAnimation;

UCLASS()
class STARCRAFT_API UStimpackDescriptionWB : public UUserWidget
{
	GENERATED_BODY()
	
private:
	bool IsStimpackButtonPressed = false;

protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* StimpackDescriptionVisibilityAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Media")
	UMediaPlayer* StimpackMediaPlayer;
	//bool IsStimpackMediaPlayerActive = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Media")
	UMediaSource* StimpackMediaSource;

public:
	virtual void SetVisibility(ESlateVisibility InVisibility) override;

	void OnStimpackPressed();
};
