// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameMenuWB.h"
#include "UI/UnitSelectMenuWB.h"
#include "Player/SC_MainCamera.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"
#include "SCUnitTestGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "AI/SCAICharacter.h"
#include "AI/SCGoalActor.h"
#include "AI/SCAIController.h"

bool UInGameMenuWB::Initialize()
{
	bool InitRes = Super::Initialize();

	if (BackToInGameMenuButton)
	{
		BackToInGameMenuButton->OnClicked.AddDynamic(this, &UInGameMenuWB::OnBackToInGameMenuButtonClicked);
	}

	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &UInGameMenuWB::OnStartGameButtonClicked);
	}

	return InitRes;
}

void UInGameMenuWB::BeginPlay()
{
}

void UInGameMenuWB::OnBackToInGameMenuButtonClicked()
{
	auto SCGameMode = Cast<ASCUnitTestGameMode>(GetWorld()->GetAuthGameMode());
	if (!SCGameMode) return;

	if (SCGameMode->GetGameState() == EGameState::INGAME_MENU)
	{
		SCGameMode->SetGameState(EGameState::INGAME_MENU);

		TArray<AActor*> AllAICharacters;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASCAICharacter::StaticClass(), AllAICharacters);
		for (int32 i = 0; i < AllAICharacters.Num(); ++i)
		{
			AllAICharacters[i]->Destroy();
		}
	}
	else if (SCGameMode->GetGameState() == EGameState::GAME)
	{
		UnitSelectMenuWB->SetVisibility(ESlateVisibility::Visible);
		StartGameButtonText->SetText(FText::FromName(StartButtonText));
		BackToInGameMenuButtonText->SetText(FText::FromName(ClearButtonText));
		SCGameMode->SetGameState(EGameState::INGAME_MENU);

		TArray<AActor*> AllAICharacters;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASCAICharacter::StaticClass(), AllAICharacters);
		for (int32 i = 0; i < AllAICharacters.Num(); ++i)
		{
			Cast<ASCAICharacter>(AllAICharacters[i])->DestroySCCharacter();
		}

		TArray<AActor*> AllAIControllers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASCAIController::StaticClass(), AllAIControllers);
		for (int32 i = 0; i < AllAIControllers.Num(); ++i)
		{
			auto AIController = Cast<ASCAIController>(AllAIControllers[i]);

			auto SCCharacter = GetWorld()->SpawnActor<ASCAICharacter>(AIController->GetCharacterStartData().CharacterClass, AIController->GetCharacterStartData().StartCharacterPosition
				, FRotator(0, AIController->GetCharacterStartData().IsCharacterFriendly ? 270 : 90, 0));
			if (!SCCharacter) return;
			SCCharacter->SetFriendly(AIController->GetCharacterStartData().IsCharacterFriendly);

			AIController->Possess(SCCharacter);
		}
	}
}

void UInGameMenuWB::OnStartGameButtonClicked()
{
	auto SCGameMode = Cast<ASCUnitTestGameMode>(GetWorld()->GetAuthGameMode());
	if (!SCGameMode) return;

	if (SCGameMode->GetGameState() == EGameState::INGAME_MENU)
	{
		StartGameButtonText->SetText(FText::FromName(AttackButtonText));
		BackToInGameMenuButtonText->SetText(FText::FromName(BackToMenuButtonText));
		UnitSelectMenuWB->SetVisibility(ESlateVisibility::Hidden);
		SCGameMode->SetGameState(EGameState::GAME);
	}
	else if (SCGameMode->GetGameState() == EGameState::GAME)
	{
		TArray<AActor*> AllAICharacters;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASCAICharacter::StaticClass(), AllAICharacters);

		for (int32 i = 0; i < AllAICharacters.Num(); ++i)
		{
			auto Character = Cast<ASCAICharacter>(AllAICharacters[i]);
			if (!Character) continue;

			Character->SetCurrentGoal(GetWorld()->SpawnActor<ASCGoalActor>(ASCGoalActor::StaticClass(), FTransform(AttackPointLocation)));
			Character->OnAddNewGoalActor(EAICharacterState::ATTACK);
		}
	}
}