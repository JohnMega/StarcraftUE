// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCAICharacter.generated.h"

class UStaticMeshComponent;
class UBehaviorTree;
class ASCPlayerController;
class UNiagaraComponent;
class UHealthComponent;
class UWidgetComponent;
class UNiagaraSystem;
class ASCGoalActor;
class ASC_MainCamera;
class USoundCue;

UENUM(BlueprintType)
enum class EAICharacterState : uint8
{
	DEFAULT, // Default cursor, defalut static attack
	ATTACK, // Attack cursor on the floor
	INDIVIDUAL_ATTACK // Attack cursor on the character
};

UCLASS()
class STARCRAFT_API ASCAICharacter : public ACharacter
{
	GENERATED_BODY()

	friend ASC_MainCamera;

private:
	ASC_MainCamera* MainCamera = nullptr;

protected:
	ASCAICharacter* AttackTargetCharacter = nullptr;

	FVector SelectedCircleDiametrVector;
	UNiagaraComponent* NiagaraFullCircleComponent = nullptr;

	ASCGoalActor* CurrentGoal = nullptr;

	EAICharacterState CharacterState = EAICharacterState::ATTACK;

	FTimerHandle StuckTimer;

	bool NiagaraComponentVisibility = false;
	bool bIsSelected = false;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UNiagaraComponent* NiagaraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Class")
	TSubclassOf<ASCAICharacter> AICharacterClass;

	UPROPERTY(EditInstanceOnly, Category = "Start Behavior")
	bool IsInitiallySelected = false;

	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FName GoalKeyName = "Goal";

	UPROPERTY(EditDefaultsOnly, Category = "Niagara")
	FName SelectedCircleRadiusVarName = "CircleRadius";

	UPROPERTY(EditDefaultsOnly, Category = "Niagara")
	FName RibbonColorVarName = "RibbonColor";

	UPROPERTY(EditDefaultsOnly, Category = "Niagara")
	FName LeftSelectedCircleSocketName = "LeftCircleSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Niagara")
	FName RightSelectedCircleSocketName = "RightCircleSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Niagara")
	UNiagaraSystem* NiagaraFullCircle;

	UPROPERTY(EditDefaultsOnly, Category = "Niagara")
	UNiagaraSystem* NiagaraBlood;

	UPROPERTY(EditAnywhere, Category = "Relationship")
	bool bIsFriendly = true;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behavior")
	UBehaviorTree* BehaviorTreeAsset;

private:
	void IsInitiallySelectedHandle();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnSkillUse(ASCAICharacter* SelectedUnit, UClass* CurrentSkillClass) { }

	ASCPlayerController* GetSCPlayerController() const;
	void OnFriendlyCharacterAim(ASCAICharacter* Character);
	void OnFriendlyCharacterClicked(ASCAICharacter* Character);
	void ActivateNiagaraCharacterCircle(ASCAICharacter* Character);
	void ActivateNiagaraCharacterFullCircle(ASCAICharacter* Character);

	void StuckHandle();
	void OnStuckTimer();

	void SetCharacterState(EAICharacterState Other);

	void AttackBehaviorHandle();
	void SearchBestEnemyToAttack();
	void LoseSightRadiusHandle();

	UFUNCTION()
	void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

public:	
	ASCAICharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void Attack() { };

	void DestroySCCharacter(); // should call instead of Destroy()
	ASCGoalActor* GetCurrentGoal() const;
	void DeleteCurrentGoal();
	bool IsSelected() const;
	void SetAttackTargetCharacter(ASCAICharacter* NewAttackTargetCharacter);
	ASCAICharacter* GetAttackTargetCharacter() const;
	FVector GetSelectedCircleDiametrVector() const;
	UNiagaraSystem* GetNiagaraBlood() const;
	UClass* GetCharacterClass() const;

	virtual void OnDeath() { }

	UFUNCTION(BlueprintCallable)
	bool IsDead() const;

	UFUNCTION(BlueprintCallable)
	void SetCurrentGoal(ASCGoalActor* NewGoal);

	UFUNCTION(BlueprintCallable)
	void OnAddNewGoalActor(EAICharacterState State);

	UFUNCTION(BlueprintCallable)
	bool IsFriendly() const;

	UFUNCTION(BlueprintCallable)
	void SetFriendly(bool IsFriendly);

	UFUNCTION(BlueprintCallable)
	bool IsInAttackState() const;

	UFUNCTION(BlueprintCallable)
	bool IsHaveAttackTarget() const;

	UFUNCTION(BlueprintCallable)
	bool IsHaveMoveGoal() const;
};
