// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWB.generated.h"

class UButton;
class UMaterialInstance;
class UMenuUnitWB;
class UPlayMenuWB;
class UUserSettingsWB;
class UWidgetAnimation;

UCLASS()
class STARCRAFT_API UMainMenuWB : public UUserWidget
{
	GENERATED_BODY()
	
private:
	TArray<UButton*> AllButtons;
	TArray<UWidget*> AllSubUserWidgets;
	TArray<UWidgetAnimation*> AllPressedAnimations;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* PlaySectionButton;

	UPROPERTY(meta = (BindWidget))
	UButton* UnitsSectionButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SettingsSectionButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitSectionButton;

	UPROPERTY(meta = (BindWidget))
	UMenuUnitWB* MenuUnitWB;

	UPROPERTY(meta = (BindWidget))
	UPlayMenuWB* MenuPlayWB;

	UPROPERTY(meta = (BindWidget))
	UUserSettingsWB* UserSettingsWB;

// Animations
protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* PlayHoveredAnim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* PlayPressedAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* UnitsHoveredAnim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* UnitsPressedAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* SettingsHoveredAnim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* SettingsPressedAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ExitHoveredAnim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ExitPressedAnim;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Materials")
	UMaterialInstance* SelectedButtonMat;

private:
	UFUNCTION()
	void OnPlaySectionButtonPressed();
	UFUNCTION()
	void OnPlaySectionButtonHovered();
	UFUNCTION()
	void OnPlaySectionButtonUnhovered();

	UFUNCTION()
	void OnUnitsSectionButtonPressed();
	UFUNCTION()
	void OnUnitsSectionButtonHovered();
	UFUNCTION()
	void OnUnitsSectionButtonUnhovered();

	UFUNCTION()
	void OnSettingsSectionButtonPressed();
	UFUNCTION()
	void OnSettingsSectionButtonHovered();
	UFUNCTION()
	void OnSettingsSectionButtonUnhovered();

	UFUNCTION()
	void OnExitSectionButtonPressed();
	UFUNCTION()
	void OnExitSectionButtonHovered();
	UFUNCTION()
	void OnExitSectionButtonUnhovered();

	void SetEnableForAllButtons(bool IsEnable);
	void SetVisibilityForAllWB(bool IsVisible);

public:
	virtual bool Initialize() override;
};
