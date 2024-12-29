// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelInfoScreenWB.generated.h"

class UButton;
class UWidgetAnimation;
class UMediaPlayer;
class UMediaSource;

UCLASS()
class STARCRAFT_API ULevelInfoScreenWB : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* InfoButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ContinueButton;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* MediaShowAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Media")
	UMediaPlayer* MinionRepairMediaPlayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Media")
	UMediaSource* MinionRepairMediaSource;

private:
	UFUNCTION()
	void OnInfoButtonClicked();

	UFUNCTION()
	void OnContinueButtonClicked();

public:
	virtual bool Initialize() override;
};
