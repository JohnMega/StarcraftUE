// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "DemoLevelScriptActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadLevelFinishSignature);

class ULoadLevelScreenWB;

UCLASS()
class STARCRAFT_API ADemoLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
private:
	ULoadLevelScreenWB* LoadLevelScreenWB;
	float PercentValue;
	float PercentStep;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sub Levels Info")
	TArray<FName> SubLevelsNames;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sub Levels Info")
	TArray<FName> SubLevelsDescriptionNames;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Load Screen Widget Class")
	TSubclassOf<ULoadLevelScreenWB> LoadLevelScreenWBClass;

public:
	UPROPERTY(BlueprintAssignable)
	FOnLoadLevelFinishSignature OnLoadLevelFinish;

private:
	UFUNCTION()
	void SubLevelLoadIteration(int32 SubLevelNumber);

	void DestroyLoadLevelScreenWB();

protected:
	virtual void BeginPlay() override;
};
