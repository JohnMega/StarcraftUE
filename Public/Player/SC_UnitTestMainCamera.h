// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/SC_MainCamera.h"
#include "SC_UnitTestMainCamera.generated.h"

enum class EGameState;

UCLASS()
class STARCRAFT_API ASC_UnitTestMainCamera : public ASC_MainCamera
{
	GENERATED_BODY()
	
private:
	AActor* StartViewTarget;
	FTimerHandle SetViewTargetWithBlendTimer;

// Input Vars
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Mapping")
	UInputMappingContext* InGameMenuMappingContext;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "View Target")
	TSubclassOf<AActor> StartViewTargetClass;

protected:
	virtual void BeginPlay() override;
	virtual void ChangeUnitSkillsKeys() override;

	void OnGameStateChange(EGameState NewGameState);
	void OnSetViewTargetWithBlendFinish();

public:
	ASC_UnitTestMainCamera(const FObjectInitializer& ObjectInitializer);

	void SetupInGameMenuInputComponentBinding(UInputComponent* PlayerInputComponent);
	void SetupGameInputComponentBinding(UInputComponent* PlayerInputComponent);
};
