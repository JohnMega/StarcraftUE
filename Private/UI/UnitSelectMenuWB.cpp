// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UnitSelectMenuWB.h"
#include "UI/InGameMenuWB.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "AI/SpawnPoint/FriendUnitsSpawnPoint.h"
#include "AI/SpawnPoint/EnemyUnitsSpawnPoint.h"
#include "AI/SCAIController.h"
#include "AI/Characters/Marine/SCMarine.h"
#include "AI/Characters/Minion/SCMinion.h"
#include "Kismet/GameplayStatics.h"
#include "SCUnitTestGameMode.h"

bool UUnitSelectMenuWB::Initialize()
{
	bool InitRes = Super::Initialize();

	if (MarineSpawnButton)
		MarineSpawnButton->OnClicked.AddDynamic(this, &UUnitSelectMenuWB::OnMarineSpawnButtonClicked);

	if (MinionSpawnButton)
		MinionSpawnButton->OnClicked.AddDynamic(this, &UUnitSelectMenuWB::OnMinionSpawnButtonClicked);

	if (BlueTeamMenuButton)
		BlueTeamMenuButton->OnClicked.AddDynamic(this, &UUnitSelectMenuWB::OnBlueTeamMenuButtonClicked);

	if (RedTeamMenuButton)
		RedTeamMenuButton->OnClicked.AddDynamic(this, &UUnitSelectMenuWB::OnRedTeamMenuButtonClicked);

	UnitSpawnButtonsTexts.Add(MarineSpawnButtonText);
	UnitSpawnButtonsTexts.Add(MinionSpawnButtonText);

	return InitRes;
}

void UUnitSelectMenuWB::OnBlueTeamMenuButtonClicked()
{
	UnitSelectMenuBackground->SetColorAndOpacity(BlueTeamBackgroundColor);
	for (int32 i = 0; i < UnitSpawnButtonsTexts.Num(); ++i) UnitSpawnButtonsTexts[i]->SetColorAndOpacity(FColor::Blue);
	TeamType = EUnitSelectMenuType::BLUE_TEAM_TYPE;
}

void UUnitSelectMenuWB::OnRedTeamMenuButtonClicked()
{
	UnitSelectMenuBackground->SetColorAndOpacity(RedTeamBackgroundColor);
	for (int32 i = 0; i < UnitSpawnButtonsTexts.Num(); ++i) UnitSpawnButtonsTexts[i]->SetColorAndOpacity(FColor::Red);
	TeamType = EUnitSelectMenuType::RED_TEAM_TYPE;
}

void UUnitSelectMenuWB::OnMarineSpawnButtonClicked()
{
	UnitSpawn<ASCMarine>(MarineClass.Get());
}

void UUnitSelectMenuWB::OnMinionSpawnButtonClicked()
{
	UnitSpawn<ASCMinion>(MinionClass.Get());
}

template <typename T>
void UUnitSelectMenuWB::UnitSpawn(UClass* UnitClass)
{
	auto SpawnPointClass = (TeamType == EUnitSelectMenuType::BLUE_TEAM_TYPE ? FriendUnitsSpawnPointClass.Get() : EnemyUnitsSpawnPointClass.Get());
	auto UnitsSpawnPoint = Cast<AFriendUnitsSpawnPoint>(UGameplayStatics::GetActorOfClass(GetWorld(), SpawnPointClass));
	if (!UnitsSpawnPoint) return;

	auto SCAIController = GetWorld()->SpawnActor<ASCAIController>(AIControllerClass.Get());
	auto SCUnit = GetWorld()->SpawnActor<T>(UnitClass, UnitsSpawnPoint->GetBestSpawnPoint()
		, FRotator(0, TeamType == EUnitSelectMenuType::BLUE_TEAM_TYPE ? 0 : 180, 0));
	if (!SCUnit) return;
	SCUnit->SetFriendly(TeamType == EUnitSelectMenuType::BLUE_TEAM_TYPE ? true : false);

	SCAIController->Possess(SCUnit);
	SCAIController->SetCharacterClass(UnitClass);

	auto SCGameMode = Cast<ASCUnitTestGameMode>(GetWorld()->GetAuthGameMode());
	if (!SCGameMode) return;

	if (TeamType == EUnitSelectMenuType::BLUE_TEAM_TYPE)
	{
		SCGameMode->SetTotalSpawnedFriendCharactersCount(SCGameMode->GetTotalSpawnedFriendCharactersCount() + 1);
	}
	else
	{
		SCGameMode->SetTotalSpawnedEnemyCharactersCount(SCGameMode->GetTotalSpawnedEnemyCharactersCount() + 1);
	}
}