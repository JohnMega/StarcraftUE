// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SCGoalActor.h"
#include "Player/SCPlayerController.h"
#include "NiagaraComponent.h"
#include "AI/SCAICharacter.h"

ASCGoalActor::ASCGoalActor()
{
	PrimaryActorTick.bCanEverTick = true;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");
	SetRootComponent(NiagaraComponent);
}

void ASCGoalActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASCGoalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASCGoalActor::SetNiagaraVisible(bool Visible)
{
	NiagaraComponent->SetVisibility(Visible);
}

void ASCGoalActor::SetNiagaraColor(EAICharacterState CharacterState)
{
	if (CharacterState == EAICharacterState::ATTACK || CharacterState == EAICharacterState::INDIVIDUAL_ATTACK) 
		NiagaraComponent->SetNiagaraVariableLinearColor(NiagaraColorVarName.ToString(), {1, 0, 0});
	else 
		NiagaraComponent->SetNiagaraVariableLinearColor(NiagaraColorVarName.ToString(), { 0, 1, 0 });
}