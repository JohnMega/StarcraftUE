// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SceneCapture2D.h"
#include "UnitTalkSceneCapture2D.generated.h"

class ASC_MainCamera;

UCLASS()
class STARCRAFT_API AUnitTalkSceneCapture2D : public ASceneCapture2D
{
	GENERATED_BODY()
	
private:
	ASC_MainCamera* MainCamera;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locations")
	TMap<UClass*, AActor*> CameraLocations;

protected:
	virtual void BeginPlay() override;

public:
	void OnSelectUnitsTalk();
};
