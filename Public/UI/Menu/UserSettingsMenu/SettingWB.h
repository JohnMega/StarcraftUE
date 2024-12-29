// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingWB.generated.h"

class UGameUserSettings;
class UButton;
class UTextBlock;
class UWidgetAnimation;
class USettingButtonWB;

UCLASS()
class STARCRAFT_API USettingWB : public UUserWidget
{
	GENERATED_BODY()
	
	static inline const TMap<int32, FString> QualityNames = { {-1, "Custom"}, {0, "Low"}, {1, "Medium"}, {2, "High"}, {3, "Epic"}, {4, "Ultra"} };

private:
	mutable UGameUserSettings* GameUserSettings;
	TArray<USettingButtonWB*> SettingButtonsWidgets;

// Widget Vars
protected:
	UPROPERTY(meta = (BindWidget))
	USettingButtonWB* LowSettingWB;

	UPROPERTY(meta = (BindWidget))
	USettingButtonWB* MediumSettingWB;

	UPROPERTY(meta = (BindWidget))
	USettingButtonWB* HighSettingWB;

	UPROPERTY(meta = (BindWidget))
	USettingButtonWB* EpicSettingWB;

	UPROPERTY(meta = (BindWidget))
	USettingButtonWB* UltraSettingWB;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SettingNameText;

	UPROPERTY(meta = (BindWidget))
	UButton* SettingHoveredButton;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* SettingHoveredAnim;

// Other Vars
protected:
	UPROPERTY()
	int32 NewSettingValue;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int32 MaxSettingValue;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	FName SettingName;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "GUS Function Names")
	FName GUSFuncGetterName;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "GUS Function Names")
	FName GUSFuncSetterName;

private:
	UFUNCTION()
	void OnSettingHoveredButtonHovered();

	UFUNCTION()
	void OnSettingHoveredButtonUnhovered();

public:
	virtual bool Initialize() override;

	void UpdateSettingText() const;

	int32 CallGUSFuncGetter() const;
	void CallGUSFuncSetter(int32 SettingValue);

	FName GetGUSFuncGetterName() const;
	FName GetGUSFuncSetterName() const;

	void OnSettingButtonClicked();
};
