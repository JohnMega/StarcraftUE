// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SCTalkMarine.generated.h"

class ASCStaticMeshWeapon;
class USkeletalMeshComponent;

UCLASS()
class STARCRAFT_API ASCTalkMarine : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ASCStaticMeshWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName WeaponSpawnSocketName = "WeaponSpawnSocket";

protected:
	virtual void BeginPlay() override;

public:
	ASCTalkMarine();
};
