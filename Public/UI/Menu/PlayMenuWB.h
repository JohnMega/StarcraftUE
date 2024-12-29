// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayMenuWB.generated.h"

class UButton;

UCLASS()
class STARCRAFT_API UPlayMenuWB : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* DemoLevelButton;

	UPROPERTY(meta = (BindWidget))
	UButton* UnitTestLevelButton;

	UPROPERTY(meta = (BindWidget))
	UWidget* UnitTestLevelPlayMenuWB;

	UPROPERTY(meta = (BindWidget))
	UWidget* DemoLevelPlayMenuWB;

private:
	UFUNCTION()
	void OnDemoLevelButtonClicked();

	UFUNCTION()
	void OnUnitTestLevelButtonButtonClicked();

public:
	virtual bool Initialize() override;

	virtual void SetVisibility(ESlateVisibility InVisibility) override;
};
