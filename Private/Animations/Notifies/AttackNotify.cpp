// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Notifies/AttackNotify.h"
#include "Kismet/GameplayStatics.h"
#include "AI/SCAICharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/HealthComponent.h"
#include "Engine/DamageEvents.h"
#include "Sound/SoundCue.h"

void UAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	auto Character = Cast<ASCAICharacter>(MeshComp->GetOwner());
	if (!Character) return;

	if (!Character->IsHaveAttackTarget() || Character->IsDead()) return;

	auto HealthComponent = Character->GetComponentByClass<UHealthComponent>();
	if (!HealthComponent) return;
	
	UNiagaraFunctionLibrary::SpawnSystemAttached(Character->GetAttackTargetCharacter()->GetNiagaraBlood(), Character->GetAttackTargetCharacter()->GetMesh(), NAME_None
		, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
	Character->GetAttackTargetCharacter()->TakeDamage(HealthComponent->GetDamage(), FDamageEvent(), Cast<AController>(Character->GetOwner()), Character);

	if (AttackSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), AttackSound, Character->GetActorLocation());
	}
}