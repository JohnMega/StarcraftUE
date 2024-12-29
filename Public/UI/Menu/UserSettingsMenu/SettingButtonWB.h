// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingButtonWB.generated.h"

class UTextBlock;
class UButton;
class UWidgetAnimation;
class USettingWB;

UCLASS()
class STARCRAFT_API USettingButtonWB : public UUserWidget
{
	GENERATED_BODY()
	
private:
	USettingWB* Owner = nullptr;
	bool bIsSelected = false;

// Widget Vars
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SettingText;

	UPROPERTY(meta = (BindWidget))
	UButton* SettingButton;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* SettingHoveredAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* SettingPressedAnim;

// Other Vars
protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Input Data")
	FName SettingTextProperty;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Input Data")
	int32 SettingValue;

private:
	UFUNCTION()
	void OnSettingButtonPressed();

	UFUNCTION()
	void OnSettingButtonHovered();

	UFUNCTION()
	void OnSettingButtonUnhovered();

public:
	virtual bool Initialize() override;

	void SetOwnerWidget(USettingWB* NewOwner);

	bool IsSettingButtonSelected() const;
	void SetSettingButtonSelected(bool NewIsSelected);
	int32 GetSettingValue() const;
};
