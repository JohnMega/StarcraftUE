// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Components/Widget.h"
#include "SCMenuHUD.generated.h"

class UMainMenuWB;
class UStimpackDescriptionWB;
class UMarineHealthKitDescriptionWB;
class UMinionHealthKitDescriptionWB;

UCLASS()
class STARCRAFT_API ASCMenuHUD : public AHUD
{
	GENERATED_BODY()
	
private:
	TArray<UWidget*> CreatedWidgets;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UMainMenuWB> MainMenuWBClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UStimpackDescriptionWB> StimpackDescriptionWBClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UMarineHealthKitDescriptionWB> MarineHealthKitDescriptionWBClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UMinionHealthKitDescriptionWB> MinionHealthKitDescriptionWBClass;

protected:
	virtual void BeginPlay() override;

public:
	template <typename T>
	T* GetWidget();
};

template <typename T>
T* ASCMenuHUD::GetWidget()
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
