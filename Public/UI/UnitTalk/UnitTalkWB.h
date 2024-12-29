// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/GeneralUITypes.h"
#include "UnitTalkWB.generated.h"

class UImage;
class UWidgetAnimation;
class ASC_MainCamera;
class ASCAICharacter;
class USoundCue;

UCLASS()
class STARCRAFT_API UUnitTalkWB : public UUserWidget
{
	GENERATED_BODY()
	
private:
	ASC_MainCamera* MainCamera;
	FTimerHandle NoiseToTalkTimer;
	UAudioComponent* LastCharacterLine = nullptr;
	TArray<UAudioComponent*> CharactersAudioLinesComponents;

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* UnitTalkImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* UnitTalkAnim;
	ELastAnimationTypePlayed UnitTalkAnimLastAnimType = ELastAnimationTypePlayed::REVERSE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Materials")
	UMaterialInstance* NoiseUnitTalkMat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Materials")
	UMaterialInstance* CleanUnitTalkMat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Audio")
	TMap<TSubclassOf<ASCAICharacter>, USoundCue*> CharactersAudioLines;

private:
	void OnSelectUnitsTalk();
	void OnClearSelectUnits();
	void OnSelectUnits();

	void OnNoiseToTalkTimer();
	void PlaySelectedCharacterAudioLines();

public:
	void BeginPlay();
};
