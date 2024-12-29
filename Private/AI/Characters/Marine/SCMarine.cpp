// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Characters/Marine/SCMarine.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/SCStaticMeshWeapon.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIController.h"
#include "Perception/AISense_Sight.h"
#include "AI/SCGoalActor.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/DamageEvents.h"
#include "Skills/MainSkillsActor.h"
#include "NiagaraFunctionLibrary.h"
#include "AI/Characters/Minion/SCMinion.h"
#include "Sound/SoundCue.h"

void ASCMarine::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnCharacterDeath.AddUObject(this, &ASCMarine::OnMarineDeath);
	
	SpawnWeapon();
	Cast<UCharacterMovementComponent>(GetMovementComponent())->RotationRate = DefaultRotationRate;

	MarineSkillsHandleMap.Add({ StimpackSkillClass.Get(), &ASCMarine::OnStimpackUsage });
	MarineSkillsHandleMap.Add({ MinionRepairSkill.MinionRepairSkillClass.Get(), &ASCMarine::OnMinionRepairUsage });
	MarineSkillsHandleMap.Add({ HealthKitSkill.HealthKitSkillClass.Get(), &ASCMarine::OnHealthKitUsage });
	AllKindMarineSkills.Add(StimpackSkillClass.Get());
	AllKindMarineSkills.Add(MinionRepairSkill.MinionRepairSkillClass.Get());
	AllKindMarineSkills.Add(HealthKitSkill.HealthKitSkillClass.Get());
}

void ASCMarine::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DestroyWeapon();

	Super::EndPlay(EndPlayReason);
}

void ASCMarine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead())
	{
		GetMesh()->AddForce({ 0,0,980 * 5 });
	}
}

void ASCMarine::OnSkillUse(ASCAICharacter* SelectedUnit, UClass* CurrentSkillClass)
{
	if (SelectedUnit == this && AllKindMarineSkills.Find(CurrentSkillClass) != INDEX_NONE)
	{
		auto CurrentSkillHandleFunc = *MarineSkillsHandleMap.Find(CurrentSkillClass);
		(this->*CurrentSkillHandleFunc)();
	}
}

void ASCMarine::OnStimpackUsage()
{
	UGameplayStatics::PlaySound2D(GetWorld(), StimpackSound);

	UNiagaraFunctionLibrary::SpawnSystemAttached(StimpackNiagara, GetMesh(), StimpackSpawnSocketName, FVector::ZeroVector, FRotator::ZeroRotator
		, EAttachLocation::SnapToTarget, true);

	MarineAnimationsSpeedRate = 1.5f;

	TakeDamage(20.0f, FDamageEvent{}, Cast<AController>(GetOwner()), this);

	GetWorld()->GetTimerManager().ClearTimer(StimpackUsageTimer);
	GetWorld()->GetTimerManager().SetTimer(StimpackUsageTimer, this, &ASCMarine::OnFinishStimpackUsage, 3.0f, false);
}

void ASCMarine::OnFinishStimpackUsage()
{
	MarineAnimationsSpeedRate = 1.0f;
}

void ASCMarine::OnMinionRepairUsage()
{
	UGameplayStatics::PlaySound2D(GetWorld(), MinionRepairSkill.MinionRepairSound);

	auto MyController = Cast<AAIController>(GetController());
	if (!MyController) return;

	TArray<AActor*> PerceivedActors;
	MyController->GetPerceptionComponent()->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
	if (!PerceivedActors.Num()) return;

	ASCMinion* BestMinion = nullptr;
	float MinDist = MAX_FLT;
	for (int32 i = 0; i < PerceivedActors.Num(); ++i)
	{
		auto SCCharacter = Cast<ASCMinion>(PerceivedActors[i]);
		if (!SCCharacter || !SCCharacter->IsFriendly() || SCCharacter->IsDead()) continue;

		float Length = (GetActorLocation() - SCCharacter->GetActorLocation()).Length();
		if (Length < MinDist)
		{
			BestMinion = SCCharacter;
			MinDist = Length;
		}
	}

	if (BestMinion && !BestMinion->IsInEnableState() && MinDist <= MinionRepairSkill.MaxSkillAvailableDistance)
	{
		BestMinion->OnMinionRepairSkillUsage(MinionRepairSkill.MinionRepairNiagara);
	}
}

void ASCMarine::OnHealthKitUsage()
{
	if (HealthKitSkill.HealthKitProgressBarValue > 0.0f) return;
	
	UGameplayStatics::PlaySound2D(GetWorld(), HealthKitSkill.HealthKitSound);

	UNiagaraFunctionLibrary::SpawnSystemAttached(HealthKitSkill.HealthKitNiagara, GetMesh(), HealthKitSkill.HealthKitSpawnSocketName, FVector::ZeroVector, FRotator::ZeroRotator
		, EAttachLocation::SnapToTarget, true);

	HealthKitSkill.HealthKitTimerUsageCount = 0;
	GetWorld()->GetTimerManager().ClearTimer(HealthKitSkill.HealthKitUsageTimer);
	GetWorld()->GetTimerManager().ClearTimer(HealthKitSkill.HealthKitCooldownTimer);
	GetWorld()->GetTimerManager().SetTimer(HealthKitSkill.HealthKitUsageTimer, this, &ASCMarine::OnHealthKitTimerUsage, 0.5f, true);
	GetWorld()->GetTimerManager().SetTimer(HealthKitSkill.HealthKitCooldownTimer, this, &ASCMarine::OnHealthKitTimerCooldown, 0.1f, true);
}

void ASCMarine::OnHealthKitTimerUsage()
{
	++HealthKitSkill.HealthKitTimerUsageCount;
	TakeDamage(-5.0f, FDamageEvent{}, Cast<AController>(GetOwner()), this);

	if (HealthKitSkill.HealthKitTimerUsageCount == HealthKitSkill.MaxHealthKitTimerUsageNum)
	{
		HealthKitSkill.HealthKitTimerUsageCount = 0;
		GetWorld()->GetTimerManager().ClearTimer(HealthKitSkill.HealthKitUsageTimer);
	}
}

void ASCMarine::OnHealthKitTimerCooldown()
{
	HealthKitSkill.HealthKitProgressBarValue = FMath::Clamp(HealthKitSkill.HealthKitProgressBarValue + HealthKitSkill.HealthKitProgressBarValueModifier, 0.0f, 1.0f);

	if (HealthKitSkill.HealthKitProgressBarValue == 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(HealthKitSkill.HealthKitCooldownTimer);
		HealthKitSkill.HealthKitProgressBarValue = 0.0f;
	}
}

void ASCMarine::OnMarineDeath()
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

void ASCMarine::SpawnWeapon()
{
	WeaponActor = GetWorld()->SpawnActor(WeaponClass.Get());
	if (WeaponActor)
	{
		FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
		WeaponActor->AttachToComponent(GetMesh(), AttachmentTransformRules, WeaponSpawnSocketName);
	}
}

void ASCMarine::OnDeath()
{
	if (!IsDead()) return;

	OnFriendlyCharacterAim(nullptr);
	OnFriendlyCharacterClicked(nullptr);
	SetCurrentGoal(nullptr);
}

void ASCMarine::DestroyWeapon()
{
	if (WeaponActor)
	{
		WeaponActor->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		WeaponActor->Destroy();
		WeaponActor = nullptr;
	}
}

void ASCMarine::Attack()
{
	if ((CurrentGoal || !AttackTargetCharacter || AttackTargetCharacter->IsDead()))
	{
		Cast<AAIController>(GetController())->ClearFocus(EAIFocusPriority::LastFocusPriority);
		Cast<UCharacterMovementComponent>(GetMovementComponent())->RotationRate = DefaultRotationRate;
		
		if(CharacterState != EAICharacterState::INDIVIDUAL_ATTACK) AttackTargetCharacter = nullptr;
	}
	else
	{
		Cast<UCharacterMovementComponent>(GetMovementComponent())->RotationRate = FireRotationRate;
		Cast<AAIController>(GetController())->SetFocus(AttackTargetCharacter);
	}
}

float ASCMarine::GetMarineAnimationsSpeedRate() const
{
	return MarineAnimationsSpeedRate;
}

const FMinionRepairSkill& ASCMarine::GetMinionRepairSkillVars() const
{
	return MinionRepairSkill;
}