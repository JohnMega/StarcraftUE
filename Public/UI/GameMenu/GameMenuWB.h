// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameMenuWB.generated.h"

class UButton;
class UOverlay;
class UUserSettingsWB;

UCLASS()
class STARCRAFT_API UGameMenuWB : public UUserWidget
{
	GENERATED_BODY()
	
// Widget Vars
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ContinueButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SettingsButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SettingsBackButton;

	UPROPERTY(meta = (BindWidget))
	UOverlay* GameMenuOverlay;

	UPROPERTY(meta = (BindWidget))
	UOverlay* SettingsBackOverlay;

	UPROPERTY(meta = (BindWidget))
	UUserSettingsWB* UserSettingsWB;

// Other Vars
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Levels Names")
	FName ManuLevelName;

private:
	UFUNCTION()
	void OnContinueButtonClicked();

	UFUNCTION()
	void OnSettingsButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();

	UFUNCTION()
	void OnSettingsBackButtonClicked();

public:
	virtual bool Initialize() override;

	void BeginPlay();
	void OnGameMenuEnable();
};
