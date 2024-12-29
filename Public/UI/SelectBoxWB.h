// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectBoxWB.generated.h"

class UImage;
class UScaleBox;

UCLASS()
class STARCRAFT_API USelectBoxWB : public UUserWidget
{
	GENERATED_BODY()

private:
	TArray<FVector2D> BoxCoords = { {0,0}, {0,0}, {0,0}, {0,0} };

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* SelectBox;

public:
	void SelectBoxTick();

	void SetBoxStartCoord(FVector2D BoxStartDotCoords);
	void SetBoxEndCoord(FVector2D BoxEndDotCoords);
	const TArray<FVector2D>& GetBoxCoords() const;
	TArray<FVector2D>& SetBoxCoords();
	void ClearBoxCoords();
};
