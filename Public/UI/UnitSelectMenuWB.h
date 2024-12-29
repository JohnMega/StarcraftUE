// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnitSelectMenuWB.generated.h"

class UButton;
class UImage;
class UTextBlock;
class ASCMarine;
class ASCMinion;
class ASCAIController;
class AFriendUnitsSpawnPoint;
class AEnemyUnitsSpawnPoint;

enum class EUnitSelectMenuType
{
	BLUE_TEAM_TYPE = 0,
	RED_TEAM_TYPE
};

UCLASS()
class STARCRAFT_API UUnitSelectMenuWB : public UUserWidget
{
	GENERATED_BODY()

private:
	TArray<UTextBlock*> UnitSpawnButtonsTexts;
	FLinearColor BlueTeamBackgroundColor = { 0.1f, 0.1f, 0.5f, 0.5f};
	FLinearColor RedTeamBackgroundColor = { 0.5f, 0.1f, 0.1f, 0.5f };
	EUnitSelectMenuType TeamType = EUnitSelectMenuType::BLUE_TEAM_TYPE;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* BlueTeamMenuButton;

	UPROPERTY(meta = (BindWidget))
	UButton* RedTeamMenuButton;

	UPROPERTY(meta = (BindWidget))
	UImage* UnitSelectMenuBackground;

	UPROPERTY(meta = (BindWidget))
	UButton* MarineSpawnButton;

	UPROPERTY(meta = (BindWidget))
	UButton* MinionSpawnButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MarineSpawnButtonText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MinionSpawnButtonText;

	UPROPERTY(EditDefaultsOnly, Category = "SpawnPoint Classes")
	TSubclassOf<AFriendUnitsSpawnPoint> FriendUnitsSpawnPointClass;

	UPROPERTY(EditDefaultsOnly, Category = "SpawnPoint Classes")
	TSubclassOf<AEnemyUnitsSpawnPoint> EnemyUnitsSpawnPointClass;

	UPROPERTY(EditDefaultsOnly, Category = "Unit Classes")
	TSubclassOf<ASCMarine> MarineClass;

	UPROPERTY(EditDefaultsOnly, Category = "Unit Classes")
	TSubclassOf<ASCMinion> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "Controller Classes")
	TSubclassOf<ASCAIController> AIControllerClass;

private:
	template <typename T>
	void UnitSpawn(UClass* UnitClass);

	UFUNCTION()
	void OnMarineSpawnButtonClicked();

	UFUNCTION()
	void OnMinionSpawnButtonClicked();

	UFUNCTION()
	void OnBlueTeamMenuButtonClicked();

	UFUNCTION()
	void OnRedTeamMenuButtonClicked();

public:
	virtual bool Initialize() override;
};
