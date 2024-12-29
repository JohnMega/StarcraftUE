// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UnitTalk/UnitTalkWB.h"
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
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

void UUnitTalkWB::BeginPlay()
{
	if (!GetWorld()) return;

	auto PlayerController = *GetWorld()->GetPlayerControllerIterator();
	if (!PlayerController.Get()) return;

	MainCamera = Cast<ASC_MainCamera>(PlayerController->GetPawn());
	if (!MainCamera) return;

	MainCamera->OnSelectUnits.AddUObject(this, &UUnitTalkWB::OnSelectUnits);
	MainCamera->OnClearSelectUnits.AddUObject(this, &UUnitTalkWB::OnClearSelectUnits);
	MainCamera->OnSelectUnitsTalk.AddUObject(this, &UUnitTalkWB::OnSelectUnitsTalk);

	for (int32 i = 0; i < CharactersAudioLines.Num(); ++i)
	{
		CharactersAudioLinesComponents.Add(UGameplayStatics::CreateSound2D(GetWorld(), CharactersAudioLines.Get(FSetElementId::FromInteger(i)).Value, 1, 1, 0, nullptr
			, false, false));
	}
}

void UUnitTalkWB::OnSelectUnitsTalk()
{
	UnitTalkImage->SetBrushFromMaterial(NoiseUnitTalkMat);
	GetWorld()->GetTimerManager().SetTimer(NoiseToTalkTimer, this, &UUnitTalkWB::OnNoiseToTalkTimer, 1.0f, false);

	PlaySelectedCharacterAudioLines();
}

void UUnitTalkWB::OnNoiseToTalkTimer()
{
	UnitTalkImage->SetBrushFromMaterial(CleanUnitTalkMat);
}

void UUnitTalkWB::OnClearSelectUnits()
{
	if (UnitTalkAnimLastAnimType == ELastAnimationTypePlayed::DEFAULT)
	{
		UnitTalkImage->SetBrushFromMaterial(NoiseUnitTalkMat);
		PlayAnimationReverse(UnitTalkAnim);
		UnitTalkAnimLastAnimType = ELastAnimationTypePlayed::REVERSE;
	}

	LastCharacterLine = nullptr;
}

void UUnitTalkWB::OnSelectUnits()
{
	if (UnitTalkAnimLastAnimType == ELastAnimationTypePlayed::REVERSE)
	{
		PlayAnimation(UnitTalkAnim);
		UnitTalkAnimLastAnimType = ELastAnimationTypePlayed::DEFAULT;
	}
}

void UUnitTalkWB::PlaySelectedCharacterAudioLines()
{
	if (LastCharacterLine)
	{
		LastCharacterLine->Stop();
	}

	for (int32 i = 0; i < CharactersAudioLines.Num(); ++i)
	{
		if (MainCamera->GetFirstSelectedUnit()->GetCharacterClass() == CharactersAudioLines.Get(FSetElementId::FromInteger(i)).Key)
		{
			CharactersAudioLinesComponents[i]->Play();
			LastCharacterLine = CharactersAudioLinesComponents[i];
			break;
		}
	}
}