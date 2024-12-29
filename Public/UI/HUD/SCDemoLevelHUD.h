// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SCDemoLevelHUD.generated.h"

class UWidget;
class ULevelInfoScreenWB;

UCLASS()
class STARCRAFT_API ASCDemoLevelHUD : public AHUD
{
	GENERATED_BODY()
	
private:
	TArray<UWidget*> CreatedWidgets;

protected:
	virtual void BeginPlay() override;

public:
	template <typename T>
	T* GetWidget();
};

template <typename T>
T* ASCDemoLevelHUD::GetWidget()
{
	T* Result = nullptr;
	for (int32 i = 0; i < CreatedWidgets.Num(); ++i)
	{
		if (Cast<T>(CreatedWidgets[i]))
		{
			Result = Cast<T>(CreatedWidgets[i]);
			break;
		}
	}

	return Result;
}