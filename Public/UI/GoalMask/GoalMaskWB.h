// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GoalMaskWB.generated.h"

class UImage;
class ASC_MainCamera;

USTRUCT(BlueprintType)
struct FGoalMaskInitVars
{
	GENERATED_USTRUCT_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* GoalActor;
};

UCLASS()
class STARCRAFT_API UGoalMaskWB : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* GoalImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Initialize")
	FGoalMaskInitVars GoalMaskInitVars;
	
protected:
	UFUNCTION(BlueprintCallable)
	void ProjectWorldToScreenWithOutsideViewPos(APlayerController const* Player, const FVector& WorldPosition, FVector2D& ScreenPosition);

public:
	UFUNCTION(BlueprintCallable)
	void InitVars(FGoalMaskInitVars NewGoalMaskInitVars);
};
