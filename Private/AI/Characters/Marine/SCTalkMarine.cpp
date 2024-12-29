// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Characters/Marine/SCTalkMarine.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapons/SCStaticMeshWeapon.h"

ASCTalkMarine::ASCTalkMarine()
{
	PrimaryActorTick.bCanEverTick = false;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	SetRootComponent(SkeletalMesh);
}

void ASCTalkMarine::BeginPlay()
{
	Super::BeginPlay();
	
	if (!GetWorld()) return;

	auto WeaponActor = GetWorld()->SpawnActor<AActor>(WeaponClass.Get());
	if (!WeaponActor) return;

	FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	WeaponActor->AttachToComponent(SkeletalMesh, AttachmentTransformRules, WeaponSpawnSocketName);
}
