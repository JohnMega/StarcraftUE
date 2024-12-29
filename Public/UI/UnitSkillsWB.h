// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/GeneralUITypes.h"
#include "UnitSkillsWB.generated.h"

class ASC_MainCamera;
class UHorizontalBox;
class UVerticalBox;
class UImage;
class UWidgetAnimation;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnVisibleUnitSkillsBoxAddChildSignature, UWidget*)

UCLASS()
class STARCRAFT_API UUnitSkillsWB : public UUserWidget
{
	GENERATED_BODY()
	
private:
	ASC_MainCamera* MainCamera;
	TMap<UClass*, UHorizontalBox*> UnitsSkillsBoxesMap;

protected:
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* VisibleUnitSkillsHorBox;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* AllUnitsSkillsBoxes;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* OpenSkillsMenu;
	ELastAnimationTypePlayed OpenSkillsMenuLastAnimType = ELastAnimationTypePlayed::REVERSE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Units Skills")
	TArray<UClass*> UnitsClasses;

public:
	FOnVisibleUnitSkillsBoxAddChildSignature OnVisibleUnitSkillsBoxAddChild;

private:
	void OnClearSelectUnits();
	void OnSelectUnitsTalk();

	void ReplaceUnitSkillsHorBox();

public:
	void BeginPlay();
};
