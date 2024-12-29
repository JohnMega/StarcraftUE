// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SCStaticMeshWeapon.generated.h"

UCLASS()
class STARCRAFT_API ASCStaticMeshWeapon : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

public:	
	ASCStaticMeshWeapon();

};
