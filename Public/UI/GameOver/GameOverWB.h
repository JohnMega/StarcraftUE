// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWB.generated.h"

class UButton;

UCLASS()
class STARCRAFT_API UGameOverWB : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* RetryButton;

	UPROPERTY(meta = (BindWidget))
	UButton* BackToMenuButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Levels Names")
	FName MenuLevelName = "MenuLevel";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Levels Names")
	FName ThisLevelName = "DemoLevel";

private:
	UFUNCTION()
	void OnRetryButtonClicked();

	UFUNCTION()
	void OnBackToMenuButtonClicked();

public:
	virtual bool Initialize() override;
};
