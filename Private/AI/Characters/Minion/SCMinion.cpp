// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Characters/Minion/SCMinion.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIController.h"
#include "AI/SCAIController.h"
#include "Perception/AISense_Sight.h"
#include "AI/SCGoalActor.h"
#include "AI/Characters/Marine/SCMarine.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/ChildActorComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DamageEvents.h"
#include "Skills/MainSkillsActor.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ASCMinion::ASCMinion()
{
	MinionRepairKeyButtonComponent = CreateDefaultSubobject<UChildActorComponent>("MinionRepairKeyButtonComponent");
	MinionRepairKeyButtonComponent->SetupAttachment(GetRootComponent());
}

void ASCMinion::BeginPlay()
{
	Super::BeginPlay();

	MinionRepairKeyButtonComponent->GetChildActor()->SetOwner(this);
	HealthComponent->OnCharacterDeath.AddUObject(this, &ASCMinion::OnMinionDeath);

	Cast<UCharacterMovementComponent>(GetMovementComponent())->RotationRate = DefaultRotationRate;
	
	MinionSkillsHandleMap.Add({ HealthKitSkillClass.Get(), &ASCMinion::OnHealthKitUsage });
	AllKindMinionSkills.Add(HealthKitSkillClass.Get());
}

void ASCMinion::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ASCMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!IsInEnableState())
		RepairKeyButtonComponentVisibleCheck();

	if (IsDead())
	{
		GetMesh()->AddForce({ 0,0,980 * 5 });
	}
}

void ASCMinion::RepairKeyButtonComponentVisibleCheck()
{
	auto MyController = Cast<AAIController>(GetController());
	if (!MyController) return;
	
	TArray<AActor*> PerceivedActors;
	MyController->GetPerceptionComponent()->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
	if (!PerceivedActors.Num()) return;

	bool IsVisible = false;
	for (int32 i = 0; i < PerceivedActors.Num(); ++i)
	{
		auto SCCharacter = Cast<ASCMarine>(PerceivedActors[i]);
		if (!SCCharacter || !SCCharacter->IsFriendly() || SCCharacter->IsDead()) continue;

		float Length = (GetActorLocation() - SCCharacter->GetActorLocation()).Length();
		if (Length < SCCharacter->GetMinionRepairSkillVars().MaxSkillAvailableDistance)
		{
			OnRepairKeyButtonComponentVisibility.Broadcast(ESlateVisibility::Visible);
			IsVisible = true;
			break;
		}
	}

	if(!IsVisible) OnRepairKeyButtonComponentVisibility.Broadcast(ESlateVisibility::Hidden);
}

void ASCMinion::OnSkillUse(ASCAICharacter* SelectedUnit, UClass* CurrentSkillClass)
{
	if (SelectedUnit == this && AllKindMinionSkills.Find(CurrentSkillClass) != INDEX_NONE)
	{
		auto CurrentSkillHandleFunc = *MinionSkillsHandleMap.Find(CurrentSkillClass);
		(this->*CurrentSkillHandleFunc)();
	}
}

void ASCMinion::OnHealthKitUsage()
{
	if (HealthKitSkillVars.HealthKitProgressBarValue > 0.0f) return;

	UGameplayStatics::PlaySound2D(GetWorld(), HealthKitSkillVars.HealthKitSound);

	UNiagaraFunctionLibrary::SpawnSystemAttached(HealthKitSkillVars.HealthKitNiagara, GetMesh(), HealthKitSkillVars.HealthKitSpawnSocketName, FVector::ZeroVector, FRotator::ZeroRotator
		, EAttachLocation::SnapToTarget, true);
	
	HealthKitSkillVars.HealthKitTimerUsageCount = 0;
	GetWorld()->GetTimerManager().ClearTimer(HealthKitSkillVars.HealthKitUsageTimer);
	GetWorld()->GetTimerManager().ClearTimer(HealthKitSkillVars.HealthKitCooldownTimer);
	GetWorld()->GetTimerManager().SetTimer(HealthKitSkillVars.HealthKitUsageTimer, this, &ASCMinion::OnHealthKitTimerUsage, 0.5f, true);
	GetWorld()->GetTimerManager().SetTimer(HealthKitSkillVars.HealthKitCooldownTimer, this, &ASCMinion::OnHealthKitTimerCooldown, 0.1f, true);
}

void ASCMinion::OnHealthKitTimerUsage()
{
	++HealthKitSkillVars.HealthKitTimerUsageCount;
	TakeDamage(-5.0f, FDamageEvent{}, Cast<AController>(GetOwner()), this);

	if (HealthKitSkillVars.HealthKitTimerUsageCount == HealthKitSkillVars.MaxHealthKitTimerUsageNum)
	{
		HealthKitSkillVars.HealthKitTimerUsageCount = 0;
		GetWorld()->GetTimerManager().ClearTimer(HealthKitSkillVars.HealthKitUsageTimer);
	}
}

void ASCMinion::OnHealthKitTimerCooldown()
{
	HealthKitSkillVars.HealthKitProgressBarValue = FMath::Clamp(HealthKitSkillVars.HealthKitProgressBarValue + HealthKitSkillVars.HealthKitProgressBarValueModifier, 0.0f, 1.0f);

	if (HealthKitSkillVars.HealthKitProgressBarValue == 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(HealthKitSkillVars.HealthKitCooldownTimer);
		HealthKitSkillVars.HealthKitProgressBarValue = 0.0f;
	}
}

bool ASCMinion::IsCanAttack() const
{
	if (!AttackTargetCharacter) return false;

	return (GetActorLocation() - AttackTargetCharacter->GetActorLocation()).Length() <= AttackTargetCharacter->GetSelectedCircleDiametrVector().Length();
}

void ASCMinion::OnMinionDeath()
{
	OnFriendlyCharacterAim(nullptr);
	OnFriendlyCharacterClicked(nullptr);
	SetCurrentGoal(nullptr);

	HealthWidgetComponent->DestroyComponent();
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(false);
}

void ASCMinion::OnDeath()
{
	if (!IsDead()) return;

	OnFriendlyCharacterAim(nullptr);
	OnFriendlyCharacterClicked(nullptr);
	SetCurrentGoal(nullptr);
}

void ASCMinion::Attack()
{
	if (!IsInEnableState()) return;

	if (AttackTargetCharacter && !AttackTargetCharacter->IsDead() && !CurrentGoal)
	{
		Cast<UCharacterMovementComponent>(GetMovementComponent())->RotationRate = AttackRotationRate;
		Cast<AAIController>(GetController())->SetFocus(AttackTargetCharacter);

		if (!IsCanAttack())
		{
			auto Blackboard = Cast<ASCAIController>(GetOwner())->GetBlackboardComponent();
			Blackboard->SetValueAsObject(GoalKeyName, AttackTargetCharacter);
		}
		else
		{
			DeleteCurrentGoal();
		}
	}
	else
	{
		Cast<AAIController>(GetController())->ClearFocus(EAIFocusPriority::LastFocusPriority);
		Cast<UCharacterMovementComponent>(GetMovementComponent())->RotationRate = DefaultRotationRate;

		if (CharacterState != EAICharacterState::INDIVIDUAL_ATTACK) AttackTargetCharacter = nullptr;
	}
}

void ASCMinion::OnMinionRepairSkillUsage(UNiagaraSystem* MinionRepairNiagara)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MinionRepairNiagara, GetActorLocation());
	MinionState = EMinionState::ENABLE;
	OnMinionEnableState.Broadcast();
}

EMinionState ASCMinion::GetMinionState() const
{
	return MinionState;
}

void ASCMinion::SetMinionState(EMinionState NewState)
{
	MinionState = NewState;
}

bool ASCMinion::IsInEnableState() const
{
	return MinionState == EMinionState::ENABLE;
}