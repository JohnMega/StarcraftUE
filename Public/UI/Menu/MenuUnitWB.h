// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuUnitWB.generated.h"

class UButton;
class UWidgetAnimation;

UCLASS()
class STARCRAFT_API UMenuUnitWB : public UUserWidget
{
	GENERATED_BODY()
	
	static const int32 TalkRaynorIndex = 0;
	static const int32 TalkMinionIndex = 1;

private:
	TArray<UButton*> AllButtons;
	TArray<UWidgetAnimation*> AllPressedAnimations;
	TArray<UWidget*> AllSubUserWidgets;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* RaynorUnitButton;

	UPROPERTY(meta = (BindWidget))
	UButton* MinionUnitButton;

	UPROPERTY(meta = (BindWidget))
	UWidget* RaynorDescriptionWB;

	UPROPERTY(meta = (BindWidget))
	UWidget* MinionDescriptionWB;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Talk Units")
	TArray<TSubclassOf<AActor>> AllTalkUnitsOnSceneClasses;

// Animations
protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* RaynorHoveredAnim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* RaynorPressedAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* MinionHoveredAnim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* MinionPressedAnim;

private:
	UFUNCTION()
	void OnRaynorUnitButtonClicked();
	UFUNCTION()
	void OnRaynorUnitButtonHovered();
	UFUNCTION()
	void OnRaynorUnitButtonUnhovered();

	UFUNCTION()
	void OnMinionUnitButtonClicked();
	UFUNCTION()
	void OnMinionUnitButtonHovered();
	UFUNCTION()
	void OnMinionUnitButtonUnhovered();

	void SetEnableForAllButtons(bool IsEnable);
	void SetVisibilityForAllWB(bool IsVisible);
	void SetActorHiddenInGameForAllUnits(bool IsVisible);

public:
	virtual bool Initialize() override;

	virtual void SetVisibility(ESlateVisibility InVisibility) override;
};
