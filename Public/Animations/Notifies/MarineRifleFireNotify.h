// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MarineRifleFireNotify.generated.h"

class UNiagaraSystem;
class USoundCue;

UCLASS()
class STARCRAFT_API UMarineRifleFireNotify : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Niagara")
	UNiagaraSystem* MuzzleFlash;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Niagara")
	FName MuzzleSocketName = "MuzzleSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
	USoundCue* RifleFireSound;

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
