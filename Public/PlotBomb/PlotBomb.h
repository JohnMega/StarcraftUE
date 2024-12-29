// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlotBomb.generated.h"

class UStaticMeshComponent;
class UWidgetComponent;
class UMaterialInstance;
class ASC_MainCamera;
class UNiagaraSystem;
class UChildActorComponent;

UCLASS()
class STARCRAFT_API APlotBomb : public AActor
{
	GENERATED_BODY()

private:
	FTimerHandle ObsCharacterDistCheckTimer;
	ASC_MainCamera* MainCamera;
	TArray<UMaterialInterface*> StaticMeshDefaultMaterials;
	bool bIsEnabledToActivate = false;
	bool bIsBombActive = false;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* WidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UChildActorComponent* BombActivateEffectSpawnLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Materials")
	UMaterialInstance* BombDisabledMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Niagara")
	UNiagaraSystem* BombActivateEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Observed Character")
	float BombActivateDistance = 250.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Observed Character")
	AActor* ObservedCharacter;

private:
	void ObsCharacterDistCheck();
	void OnBombActivate();
	void GetMainCamera();

protected:
	virtual void BeginPlay() override;

public:	
	APlotBomb();

	bool IsBombActive() const;
};
