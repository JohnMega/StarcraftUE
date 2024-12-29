// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWB.generated.h"

class UHorizontalBox;
class UImage;
class UProgressBar;

UCLASS()
class STARCRAFT_API UHealthBarWB : public UUserWidget
{
	GENERATED_BODY()

private:
	TArray<UImage*> HealthBarParts;
	float HealthBarPartSize = 25.0f;
	float CurrentHealth = 0.0f;

protected:
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HealthHorizontalBox;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;

public:
	virtual bool Initialize() override;

	void SetHealthPercent(float HealthPercent);
};
