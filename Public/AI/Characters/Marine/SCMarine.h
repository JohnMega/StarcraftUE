// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SCAICharacter.h"
#include "SCMarine.generated.h"

class ASCStaticMeshWeapon;
class AMainSkillsActor;
class UNiagaraSystem;
class USoundCue;

USTRUCT(BlueprintType)
struct FMinionRepairSkill
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UNiagaraSystem* MinionRepairNiagara;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AMainSkillsActor> MinionRepairSkillClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundCue* MinionRepairSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxSkillAvailableDistance = 25.0f;
};

USTRUCT(BlueprintType)
struct FHealthKitSkillMarine
{
	GENERATED_USTRUCT_BODY()

public:
	int32 HealthKitTimerUsageCount = 0;
	float HealthKitProgressBarValue = 0.0f;
	FTimerHandle HealthKitCooldownTimer;
	FTimerHandle HealthKitUsageTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HealthKit")
	TSubclassOf<AMainSkillsActor> HealthKitSkillClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HealthKit", meta = (ClampMin = "0", ClampMax = "100"))
	int32 MaxHealthKitTimerUsageNum;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HealthKit", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float HealthKitProgressBarValueModifier = 0.01f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HealthKit")
	UNiagaraSystem* HealthKitNiagara;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HealthKit")
	FName HealthKitSpawnSocketName = "StimpackSpawnSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HealthKit")
	USoundCue* HealthKitSound;
};

UCLASS()
class STARCRAFT_API ASCMarine : public ASCAICharacter
{
	GENERATED_BODY()

private:
	AActor* WeaponActor = nullptr;

	FTimerHandle StimpackUsageTimer;
	float MarineAnimationsSpeedRate = 1.0f;

// Unit Skills Vars
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Marine Skills")
	TSubclassOf<AMainSkillsActor> StimpackSkillClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Marine Skills")
	FMinionRepairSkill MinionRepairSkill;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Marine Skills")
	FHealthKitSkillMarine HealthKitSkill;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stimpack")
	UNiagaraSystem* StimpackNiagara;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stimpack")
	FName StimpackSpawnSocketName = "StimpackSpawnSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stimpack")
	USoundCue* StimpackSound;

	TMap<TSubclassOf<AMainSkillsActor>, void(ASCMarine::*)()> MarineSkillsHandleMap;
	TArray<TSubclassOf<AMainSkillsActor>> AllKindMarineSkills;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ASCStaticMeshWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName WeaponSpawnSocketName = "WeaponSpawnSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rotation")
	FRotator DefaultRotationRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rotation")
	FRotator FireRotationRate;

private:
	void SpawnWeapon();
	void DestroyWeapon();
	void OnMarineDeath();

	void OnStimpackUsage();
	void OnFinishStimpackUsage();

	void OnHealthKitUsage();
	void OnHealthKitTimerUsage();
	void OnHealthKitTimerCooldown();

	void OnMinionRepairUsage();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnSkillUse(ASCAICharacter* SelectedUnit, UClass* CurrentSkillClass) override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void Attack() override;
	virtual void OnDeath() override;

	const FMinionRepairSkill& GetMinionRepairSkillVars() const;

	UFUNCTION(BlueprintCallable)
	float GetMarineAnimationsSpeedRate() const;
};
