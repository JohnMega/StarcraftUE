// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MainCameraWidgetComponent.generated.h"

class USelectBoxWB;
class UInGameMenuWB;
class UUnitSkillsWB;
class UUnitTalkWB;
class UMinimapWB;
class UGameMenuWB;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STARCRAFT_API UMainCameraWidgetComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	TArray<UUserWidget*> CreatedWidgets;
	USelectBoxWB* SelectBoxWidget;
	UUnitTalkWB* UnitTalkWidget;
	UUnitSkillsWB* UnitSkillsWidget;
	UMinimapWB* MinimapWidget;
	UGameMenuWB* GameMenuWidget;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<USelectBoxWB> SelectBoxClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UUnitTalkWB> UnitTalkClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UUnitSkillsWB> UnitSkillsClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UMinimapWB> MinimapClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UGameMenuWB> GameMenuClass;

protected:
	virtual void BeginPlay() override;

public:	
	UMainCameraWidgetComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	template <typename T>
	T* GetWidget();
};

template <typename T>
T* UMainCameraWidgetComponent::GetWidget()
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
