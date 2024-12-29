// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MinimapWB.generated.h"

class UButton;
class UImage;
class AMinimap;

UCLASS()
class STARCRAFT_API UMinimapWB : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* MinimapMoveButton;

	UPROPERTY(meta = (BindWidget))
	UImage* MinimapImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Classes")
	TSubclassOf<AMinimap> MinimapClass;

private:
	UFUNCTION()
	void OnMinimapMoveButtonClicked();

public:
	virtual bool Initialize() override;
};
