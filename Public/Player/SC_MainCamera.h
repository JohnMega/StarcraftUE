// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SC_MainCamera.generated.h"

class UInputAction;
struct FInputActionValue;
class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;
class UCameraComponent;
class UNiagaraComponent;
class UCameraZoomComponent;
class UMainCameraMovementComponent;
class UMainCameraWidgetComponent;
class ASCGoalActor;
class ASCAICharacter;
class ACameraMoveZone;
class AMainSkillsActor;
enum class EGameState;

enum class EMainCameraStates
{
	DEFAULT,
	ATTACK,
	INDIVIDUAL_ATTACK
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSkillUseSignature, ASCAICharacter*, UClass*)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnFriendlyCharacterAimSignature, ASCAICharacter*)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnFriendlyCharacterClickedSignature, ASCAICharacter*)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStateChangeSignature, EMainCameraStates)
DECLARE_MULTICAST_DELEGATE(FOnSelectUnitsSignature)
DECLARE_MULTICAST_DELEGATE(FOnClearSelectUnitsSignature)
DECLARE_MULTICAST_DELEGATE(FOnSelectUnitsTalkSignature)
DECLARE_MULTICAST_DELEGATE(FOnBombActivateSignature)

UCLASS()
class STARCRAFT_API ASC_MainCamera : public APawn
{
	GENERATED_BODY()

	friend ASCAICharacter;

protected:
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = nullptr;

	FHitResult CursorHitResult;
	TArray<ASCAICharacter*> SelectedUnits;
	TArray<ASCAICharacter*> AimedUnits;
	EMainCameraStates CurrentState = EMainCameraStates::DEFAULT;

	bool bIsInSelectSeveralUnitsMode = false;
	bool bIsInSelectBoxMode = false;
	bool bTest = false;
	
// Input Vars
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Mapping")
	TMap<TSubclassOf<ASCAICharacter>, UInputMappingContext*> UnitSkillsMappingContextMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Mapping")
	UInputMappingContext* GameMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Action")
	UInputAction* IACameraMove;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Action")
	UInputAction* IAAttackState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Action")
	UInputAction* IACameraDecrease;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Action")
	UInputAction* IACameraZoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Action")
	UInputAction* IACreateGoal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Action")
	UInputAction* IASelectBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Action")
	UInputAction* IASelectOneUnit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Action")
	UInputAction* IASelectSeveralUnits;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Action")
	UInputAction* IASelectUnitsFinish;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Action")
	UInputAction* IAUseStimpack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Action")
	UInputAction* IAUseHealthKit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Action")
	UInputAction* IAMinionRepair;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Action")
	UInputAction* IAGameMenuEnable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Action")
	UInputAction* IABombActivate;

// Units Skills Vars
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Units Skills")
	TSubclassOf<AMainSkillsActor> StimpackSkill;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Units Skills")
	TSubclassOf<AMainSkillsActor> HealthKitSkill;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Units Skills")
	TSubclassOf<AMainSkillsActor> MinionRepairSkill;

// Other
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UNiagaraComponent* CameraDynamicMaskNiagara;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UCameraZoomComponent* CameraZoomComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UMainCameraMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UMainCameraWidgetComponent* MainCameraWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Goal")
	FName CharacterInGoalSpawnDotName = "FullCircleSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Goal")
	TSubclassOf<ASCGoalActor> GoalActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	bool IsMoveOnMouseAvailable = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Move Zone")
	TSubclassOf<ACameraMoveZone> CameraMoveZoneClass;

public:
	FOnFriendlyCharacterAimSignature OnFriendlyCharacterAim;
	FOnFriendlyCharacterClickedSignature OnFriendlyCharacterClicked;
	FOnStateChangeSignature OnStateChange;
	FOnSelectUnitsSignature OnSelectUnits;
	FOnClearSelectUnitsSignature OnClearSelectUnits;
	FOnSelectUnitsTalkSignature OnSelectUnitsTalk;
	FOnSkillUseSignature OnSkillUse;
	FOnBombActivateSignature OnBombActivate;

private:
	void LineTraceFromCursorForPawn();

	void OnCameraMove(const FInputActionValue& Value);
	void OnCreateGoal(const FInputActionValue& Value);
	void OnSelectBox(const FInputActionValue& Value);
	void OnSelectOneUnitPressed(const FInputActionValue& Value);
	void OnAttackState(const FInputActionValue& Value);
	void OnSelectOneUnitReleased(const FInputActionValue& Value);
	void OnSelectSeveralUnitsPressed(const FInputActionValue& Value);
	void OnSelectSeveralUnitsReleased(const FInputActionValue& Value);
	void CameraZoom(const FInputActionValue& Value);
	void CameraDecrease(const FInputActionValue& Value);
	void OnGameMenuEnable(const FInputActionValue& Value);
	void OnBombActivateAction(const FInputActionValue& Value);

	void OnUseStimpack(const FInputActionValue& Value);
	void OnUseHealthKit(const FInputActionValue& Value);
	void OnUseMinionRepair(const FInputActionValue& Value);

protected:
	virtual void BeginPlay() override;
	virtual void ChangeUnitSkillsKeys();

public:	
	ASC_MainCamera(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	EMainCameraStates GetCurrentState() const;

	bool IsSomeoneUnitSelected() const;
	const TArray<ASCAICharacter*>* GetSelectedUnits() const;

	UFUNCTION(BlueprintCallable)
	const ASCAICharacter* GetFirstSelectedUnit() const;
};