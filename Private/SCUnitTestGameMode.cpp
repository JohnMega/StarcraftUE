// Fill out your copyright notice in the Description page of Project Settings.


#include "SCUnitTestGameMode.h"
#include "Player/SC_UnitTestMainCamera.h"
#include "Player/SCPlayerController.h"

ASCUnitTestGameMode::ASCUnitTestGameMode()
{
	DefaultPawnClass = ASC_UnitTestMainCamera::StaticClass();
	PlayerControllerClass = ASCPlayerController::StaticClass();
}

void ASCUnitTestGameMode::SetGameState(EGameState NewGameState)
{
	CurrentGameState = NewGameState;
	OnGameStateChange.Broadcast(CurrentGameState);
}

EGameState ASCUnitTestGameMode::GetGameState() const
{
	return CurrentGameState;
}

int32 ASCUnitTestGameMode::GetTotalSpawnedFriendCharactersCount() const
{
	return TotalSpawnedFriendCharactersCount;
}

void ASCUnitTestGameMode::SetTotalSpawnedFriendCharactersCount(int32 NewCount)
{
	TotalSpawnedFriendCharactersCount = NewCount;
}

int32 ASCUnitTestGameMode::GetTotalSpawnedEnemyCharactersCount() const
{
	return TotalSpawnedEnemyCharactersCount;
}

void ASCUnitTestGameMode::SetTotalSpawnedEnemyCharactersCount(int32 NewCount)
{
	TotalSpawnedEnemyCharactersCount = NewCount;
}