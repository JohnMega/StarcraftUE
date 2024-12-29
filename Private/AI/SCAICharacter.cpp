// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SCAICharacter.h"
#include "Components/StaticMeshComponent.h"
#include "AI/SCAIController.h"
#include "Player/SCPlayerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/SCGoalActor.h"
#include "NiagaraComponent.h"
#include "Player/SC_MainCamera.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "Components/HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

ASCAICharacter::ASCAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->bReturnMaterialOnMove = true;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");
	NiagaraComponent->SetupAttachment(GetRootComponent());

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	HealthWidgetComponent->SetupAttachment(GetRootComponent());
}

void ASCAICharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!GetWorld()) return;

	auto PlayerController = GetSCPlayerController();
	if (!PlayerController) return;

	MainCamera = Cast<ASC_MainCamera>(PlayerController->GetPawn());
	if (MainCamera)
	{
		MainCamera->OnFriendlyCharacterAim.AddUObject(this, &ASCAICharacter::OnFriendlyCharacterAim);
		MainCamera->OnFriendlyCharacterClicked.AddUObject(this, &ASCAICharacter::OnFriendlyCharacterClicked);
		MainCamera->OnSkillUse.AddUObject(this, &ASCAICharacter::OnSkillUse);
	}

	OnActorHit.AddDynamic(this, &ASCAICharacter::OnHit);
	
	SelectedCircleDiametrVector = GetMesh()->GetSocketLocation(LeftSelectedCircleSocketName) - GetMesh()->GetSocketLocation(RightSelectedCircleSocketName);
	NiagaraComponent->SetNiagaraVariableFloat(SelectedCircleRadiusVarName.ToString(), (SelectedCircleDiametrVector.Length() / 2.0f));
	
	IsInitiallySelectedHandle();
}

void ASCAICharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//DeleteCurrentGoal() - Current goal deletes in controller
	ActivateNiagaraCharacterCircle(nullptr);
	ActivateNiagaraCharacterFullCircle(nullptr);

	Super::EndPlay(EndPlayReason);
}

void ASCAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StuckHandle();

	if (CharacterState == EAICharacterState::INDIVIDUAL_ATTACK)
	{
		if (AttackTargetCharacter) AttackBehaviorHandle();
	}
	else
	{
		if(AttackTargetCharacter)
			AttackBehaviorHandle();
		else
			SearchBestEnemyToAttack();
	}
}

void ASCAICharacter::IsInitiallySelectedHandle()
{
	if (IsInitiallySelected)
	{
		MainCamera->SelectedUnits.Add(this);
		MainCamera->OnFriendlyCharacterClicked.Broadcast(this);
		MainCamera->OnSelectUnitsTalk.Broadcast();
		MainCamera->OnSelectUnits.Broadcast();
	}
}

void ASCAICharacter::DestroySCCharacter()
{
	Cast<ASCAIController>(GetOwner())->UnPossess();
	Destroy();
}

void ASCAICharacter::AttackBehaviorHandle()
{
	if (CharacterState == EAICharacterState::DEFAULT)
	{
		LoseSightRadiusHandle();
	}
	else if (CharacterState == EAICharacterState::ATTACK)
	{
		DeleteCurrentGoal();
		CharacterState = EAICharacterState::DEFAULT;

		LoseSightRadiusHandle();
	}
	else if (CharacterState == EAICharacterState::INDIVIDUAL_ATTACK)
	{
		LoseSightRadiusHandle();
	}

	Attack();
}

void ASCAICharacter::SearchBestEnemyToAttack()
{
	if (CharacterState == EAICharacterState::INDIVIDUAL_ATTACK) return;

	auto MyController = Cast<AAIController>(GetController());
	if (!MyController) return;

	TArray<AActor*> PerceivedActors;
	MyController->GetPerceptionComponent()->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
	if (!PerceivedActors.Num()) return;

	ASCAICharacter* BestActor = nullptr;
	float MinDist = MAX_FLT;
	for (int32 i = 0; i < PerceivedActors.Num(); ++i)
	{
		auto SCCharacter = Cast<ASCAICharacter>(PerceivedActors[i]);
		if (!SCCharacter || IsFriendly() == SCCharacter->IsFriendly() || SCCharacter->IsDead()) continue;

		float Length = (GetActorLocation() - SCCharacter->GetActorLocation()).Length();
		if (Length < MinDist)
		{
			BestActor = SCCharacter;
			MinDist = Length;
		}
	}

	AttackTargetCharacter = BestActor;
	if (!bIsFriendly && AttackTargetCharacter) DeleteCurrentGoal(); // For AI`s
}

void ASCAICharacter::LoseSightRadiusHandle()
{
	auto MyController = Cast<AAIController>(GetController());
	if (!MyController) return;

	float LoseSightRad = Cast<UAISenseConfig_Sight>(*MyController->GetPerceptionComponent()->GetSensesConfigIterator())->LoseSightRadius;
	if ((AttackTargetCharacter->GetActorLocation() - GetActorLocation()).Length() >= LoseSightRad - 5)
	{
		if (!CurrentGoal)
		{
			auto GoalActor = GetWorld()->SpawnActorDeferred<ASCGoalActor>(ASCGoalActor::StaticClass(), AttackTargetCharacter->GetActorTransform());
			GoalActor->SetNiagaraVisible(false);
			GoalActor->FinishSpawning(AttackTargetCharacter->GetActorTransform());

			SetCurrentGoal(GoalActor);
			OnAddNewGoalActor(CharacterState);
		}

		if(CharacterState != EAICharacterState::INDIVIDUAL_ATTACK) AttackTargetCharacter = nullptr;
	}
	else if (CharacterState == EAICharacterState::INDIVIDUAL_ATTACK)
	{
		DeleteCurrentGoal();
		CharacterState = EAICharacterState::DEFAULT;
	}
}

void ASCAICharacter::StuckHandle()
{
	if (GetVelocity().Length() < GetMovementComponent()->GetMaxSpeed() && CurrentGoal)
	{
		if (!GetWorldTimerManager().IsTimerActive(StuckTimer))
			GetWorldTimerManager().SetTimer(StuckTimer, this, &ASCAICharacter::OnStuckTimer, 1.0f, false);
	}
}

void ASCAICharacter::OnStuckTimer()
{
	if (GetVelocity().Length() < GetMovementComponent()->GetMaxSpeed() / 1.5f)
	{
		DeleteCurrentGoal();
	}
}

void ASCAICharacter::DeleteCurrentGoal()
{
	SetCurrentGoal(nullptr);
	auto Blackboard = Cast<ASCAIController>(GetOwner())->GetBlackboardComponent();
	Blackboard->SetValueAsObject(GoalKeyName, nullptr);
}

void ASCAICharacter::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	OtherActor->SetActorLocation(-Hit.ImpactNormal + OtherActor->GetActorLocation());
}

ASCPlayerController* ASCAICharacter::GetSCPlayerController() const
{
	auto PlayerController = Cast<ASCPlayerController>(GetWorld()->GetPlayerControllerIterator()->Get());
	return PlayerController;
}

void ASCAICharacter::OnAddNewGoalActor(EAICharacterState State)
{
	auto Blackboard = Cast<ASCAIController>(GetOwner())->GetBlackboardComponent();
	Blackboard->SetValueAsObject(GoalKeyName, CurrentGoal);

	GetWorldTimerManager().ClearTimer(StuckTimer);

	CharacterState = State;
	CurrentGoal->SetNiagaraColor(CharacterState);
}

void ASCAICharacter::OnFriendlyCharacterAim(ASCAICharacter* Character)
{
	ActivateNiagaraCharacterCircle(Character);
}

void ASCAICharacter::OnFriendlyCharacterClicked(ASCAICharacter* Character)
{
	ActivateNiagaraCharacterFullCircle(Character);
}

void ASCAICharacter::ActivateNiagaraCharacterCircle(ASCAICharacter* Character)
{
	if (bIsSelected && Character) return;

	if (Character == this)
	{
		if (bIsFriendly)
			NiagaraComponent->SetNiagaraVariableLinearColor(RibbonColorVarName.ToString(), FColor::Green);
		else
			NiagaraComponent->SetNiagaraVariableLinearColor(RibbonColorVarName.ToString(), FColor::Red);

		NiagaraComponent->SetPaused(false);
		NiagaraComponent->SetVisibility(true);
	}
	else
	{
		NiagaraComponent->SetPaused(true);
		NiagaraComponent->SetVisibility(false);
	}
}

void ASCAICharacter::ActivateNiagaraCharacterFullCircle(ASCAICharacter* Character)
{
	if (!bIsFriendly) return;

	if (bIsSelected)
	{
		if (!Character)
		{
			bIsSelected = false;

			if (NiagaraFullCircleComponent)
			{
				NiagaraFullCircleComponent->DestroyComponent();
				NiagaraFullCircleComponent = nullptr;
			}
		}
	}
	else
	{
		if (Character == this)
		{
			bIsSelected = true;

			NiagaraFullCircleComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraFullCircle, GetRootComponent(), " ", { 0,0, NiagaraComponent->GetComponentLocation().Z - GetActorLocation().Z }, FRotator::ZeroRotator
				, EAttachLocation::SnapToTarget, true);

			NiagaraFullCircleComponent->SetNiagaraVariableFloat(SelectedCircleRadiusVarName.ToString(), (SelectedCircleDiametrVector.Length() / 2.0f));

			ActivateNiagaraCharacterCircle(nullptr);
		}
	}
}

void ASCAICharacter::SetCharacterState(EAICharacterState Other)
{
	CharacterState = Other;
}

ASCGoalActor* ASCAICharacter::GetCurrentGoal() const
{
	return CurrentGoal;
}

void ASCAICharacter::SetCurrentGoal(ASCGoalActor* NewGoal)
{
	if (CurrentGoal) CurrentGoal->Destroy();
	CurrentGoal = NewGoal;
}

FVector ASCAICharacter::GetSelectedCircleDiametrVector() const
{
	return SelectedCircleDiametrVector;
}

bool ASCAICharacter::IsFriendly() const
{
	return bIsFriendly;
}

void ASCAICharacter::SetFriendly(bool IsFriendly)
{
	bIsFriendly = IsFriendly;
}

bool ASCAICharacter::IsSelected() const
{
	return bIsSelected;
}

UNiagaraSystem* ASCAICharacter::GetNiagaraBlood() const
{
	return NiagaraBlood;
}

ASCAICharacter* ASCAICharacter::GetAttackTargetCharacter() const
{
	return AttackTargetCharacter;
}

void ASCAICharacter::SetAttackTargetCharacter(ASCAICharacter* NewAttackTargetCharacter)
{
	AttackTargetCharacter = NewAttackTargetCharacter;
}

bool ASCAICharacter::IsHaveAttackTarget() const
{
	return !!AttackTargetCharacter;
}

bool ASCAICharacter::IsHaveMoveGoal() const
{
	return !!CurrentGoal;
}

bool ASCAICharacter::IsInAttackState() const
{
	return CharacterState == EAICharacterState::ATTACK || CharacterState == EAICharacterState::INDIVIDUAL_ATTACK;
}

bool ASCAICharacter::IsDead() const
{
	return HealthComponent->IsDead();
}

UClass* ASCAICharacter::GetCharacterClass() const
{
	return AICharacterClass;
}