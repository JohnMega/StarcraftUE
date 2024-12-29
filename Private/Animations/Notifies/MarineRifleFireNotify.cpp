// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Notifies/MarineRifleFireNotify.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AI/SCAICharacter.h"
#include "Components/HealthComponent.h"
#include "Engine/DamageEvents.h"
#include "Sound/SoundCue.h"

void UMarineRifleFireNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	auto Character = Cast<ASCAICharacter>(MeshComp->GetOwner());
	if (!Character) return;

	if (!Character->IsHaveAttackTarget() || Character->IsDead()) return;

	UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFlash, MeshComp, MuzzleSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);

	if (RifleFireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), RifleFireSound, Character->GetActorLocation());
	}
}
