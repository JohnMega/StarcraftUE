// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FogOfWar.generated.h"

class UStaticMeshComponent;
class ASCAICharacter;

UCLASS()
class STARCRAFT_API AFogOfWar : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Materials")
	UMaterialInstance* FogOfWarTraceMat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Materials")
	UMaterialInstance* FogOfWarMainCircleMat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Materials")
	FName LocationParamName = "Location";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Materials")
	UTextureRenderTarget2D* FogOfWarTraceRenderTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Materials")
	UTextureRenderTarget2D* FogOfWarMainCircleRenderTarget;

	UPROPERTY(BlueprintReadWrite, Category = "Materials")
	UMaterialInstanceDynamic* FogOfWarTraceMatDynamic;

	UPROPERTY(BlueprintReadWrite, Category = "Materials")
	UMaterialInstanceDynamic* FogOfWarMainCircleMatDynamic;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Classes")
	TArray<TSubclassOf<ASCAICharacter>> AllFriendUnitsClasses;

protected:
	virtual void BeginPlay() override;

public:	
	AFogOfWar();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void DrawTraceOnFogOfWar(const FVector& UnitLocation);
};
