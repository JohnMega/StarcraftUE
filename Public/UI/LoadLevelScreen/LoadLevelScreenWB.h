// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadLevelScreenWB.generated.h"

class UProgressBar;
class UTextBlock;
class UImage;

struct FLoadLevelIterationInfo
{
	float PercentValue;
	FName CurrentSubLevelDescription;
};

UCLASS()
class STARCRAFT_API ULoadLevelScreenWB : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UImage* LoadLevelScreenBackground;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* LoadLevelProgress;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LoadLevelDescriptionText;

public:
	void LoadLevelNextIteration(FLoadLevelIterationInfo LoadLevelIterationInfo);
};
