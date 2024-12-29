// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SCTalkingMinion.generated.h"

class USkeletalMeshComponent;

UCLASS()
class STARCRAFT_API ASCTalkingMinion : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMesh;

public:
	ASCTalkingMinion();
};
