// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/GeneralUITypes.h"
#include "InGameMenuWB.generated.h"

class UButton;
class UImage;
class UTextBlock;
class UWidgetAnimation;
class UUnitSelectMenuWB;
class ASC_MainCamera;

UCLASS()
class STARCRAFT_API UInGameMenuWB : public UUserWidget
{
	GENERATED_BODY()
	
private:
	ASC_MainCamera* MainCamera;
	bool UnitSelectMenuVisibility = false;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* BackToInGameMenuButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BackToInGameMenuButtonText;

	UPROPERTY(meta = (BindWidget))
	UButton* StartGameButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StartGameButtonText;

	UPROPERTY(meta = (BindWidget))
	UUnitSelectMenuWB* UnitSelectMenuWB;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack Point Location")
	FVector AttackPointLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Texts")
	FName BackToMenuButtonText = "Back";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Texts")
	FName ClearButtonText = "Clear";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Texts")
	FName AttackButtonText = "Attack";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Texts")
	FName StartButtonText = "Start";

private:
	UFUNCTION()
	void OnBackToInGameMenuButtonClicked();

	UFUNCTION()
	void OnStartGameButtonClicked();

public:
	virtual bool Initialize() override;

	void BeginPlay();
};
