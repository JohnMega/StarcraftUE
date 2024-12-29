// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SCAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "AI/SCAICharacter.h"

ASCAIController::ASCAIController()
{
	MyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
	SetPerceptionComponent(*MyPerceptionComponent);
}

void ASCAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	const auto STUCharacter = Cast<ASCAICharacter>(InPawn);
	if (STUCharacter)
	{
		RunBehaviorTree(STUCharacter->BehaviorTreeAsset);
		CharacterStartData.StartCharacterPosition = STUCharacter->GetActorLocation();
		CharacterStartData.IsCharacterFriendly = STUCharacter->IsFriendly();
	}
}

void ASCAIController::OnUnPossess()
{
	const auto STUCharacter = Cast<ASCAICharacter>(GetPawn());
	if (STUCharacter)
	{
		STUCharacter->DeleteCurrentGoal();
	}

	Super::OnUnPossess();
}

const FCharacterStartData& ASCAIController::GetCharacterStartData() const
{
	return CharacterStartData;
}

void ASCAIController::SetCharacterClass(UClass* CharacterClass)
{
	CharacterStartData.CharacterClass = CharacterClass;
}