// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Characters/Minion/SCTalkingMinion.h"
#include "Components/SkeletalMeshComponent.h"

ASCTalkingMinion::ASCTalkingMinion()
{
	PrimaryActorTick.bCanEverTick = false;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	SkeletalMesh->SetupAttachment(GetRootComponent());
}