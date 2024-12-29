// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SCAICharacter.h"
#include "SCMinion.generated.h"

class AMainSkillsActor;
class UNiagaraSystem;
class UChildActorComponent;
class USoundCue;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnRepairKeyButtonComponentVisibilitySignature, ESlateVisibility)
DECLARE_MULTICAST_DELEGATE(FOnMinionEnableStateSignature)

UENUM(BlueprintType)
enum class EMinionState : uint8
{
	ENABLE,
	DISABLE
};

USTRUCT(BlueprintType)
struct FHealthKitSkillMinion
{
	GENERATED_USTRUCT_BODY()

public:
	int32 HealthKitTimerUsageCount = 0;
	float HealthKitProgressBarValue = 0.0f;
	FTimerHandle HealthKitCooldownTimer;
	FTimerHandle HealthKitUsageTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HealthKit", meta = (ClampMin = "0", ClampMax = "100"))
	int32 MaxHealthKitTimerUsageNum;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HealthKit", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float HealthKitProgressBarValueModifier = 0.01f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HealthKit")
	UNiagaraSystem* HealthKitNiagara;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HealthKit")
	FName HealthKitSpawnSocketName = "HealthKitSpawnSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HealthKit")
	USoundCue* HealthKitSound;
};

UCLASS()
class STARCRAFT_API ASCMinion : public ASCAICharacter
{
	GENERATED_BODY()
	
private:
	AActor* WeaponActor = nullptr;

// Unit Skills Vars
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Minion Skills")
	TSubclassOf<AMainSkillsActor> HealthKitSkillClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Minion Skills")
	FHealthKitSkillMinion HealthKitSkillVars;

	TMap<TSubclassOf<AMainSkillsActor>, void(ASCMinion::*)()> MinionSkillsHandleMap;
	TArray<TSubclassOf<AMainSkillsActor>> AllKindMinionSkills;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Component")
	UChildActorComponent* MinionRepairKeyButtonComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName WeaponSpawnSocketName = "WeaponSpawnSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rotation")
	FRotator DefaultRotationRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rotation")
	FRotator AttackRotationRate;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Minion State")
	EMinionState MinionState = EMinionState::ENABLE;

public:
	FOnRepairKeyButtonComponentVisibilitySignature OnRepairKeyButtonComponentVisibility;
	FOnMinionEnableStateSignature OnMinionEnableState;

private:
	void OnMinionDeath();

	void OnHealthKitUsage();
	void OnHealthKitTimerUsage();
	void OnHealthKitTimerCooldown();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnSkillUse(ASCAICharacter* SelectedUnit, UClass* CurrentSkillClass) override;

public:
	ASCMinion();

	virtual void Tick(float DeltaTime) override;

	virtual void Attack() override;
	virtual void OnDeath() override;

	EMinionState GetMinionState() const;

	// Used with Marine MinionRepair Skill
	void OnMinionRepairSkillUsage(UNiagaraSystem* MinionRepairNiagara);

	void RepairKeyButtonComponentVisibleCheck();

	UFUNCTION(BlueprintCallable)
	bool IsInEnableState() const;

	UFUNCTION(BlueprintCallable)
	void SetMinionState(EMinionState NewState);

	UFUNCTION(BlueprintCallable)
	bool IsCanAttack() const;
};
