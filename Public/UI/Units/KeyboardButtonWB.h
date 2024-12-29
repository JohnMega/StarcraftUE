// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/GeneralUITypes.h"
#include "KeyboardButtonWB.generated.h"

class UWidgetAnimation;

UCLASS()
class STARCRAFT_API UKeyboardButtonWB : public UUserWidget
{
	GENERATED_BODY()
	
private:
	ELastAnimationTypePlayed LATPWidgetVisibleAnim = ELastAnimationTypePlayed::REVERSE;

protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* WidgetVisibleAnim;

public:
	virtual void SetVisibility(ESlateVisibility InVisibility) override;
};
