// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SC_UnitTestMainCamera.h"
#include "Kismet/GameplayStatics.h"
#include "SCUnitTestGameMode.h"
#include "Camera/CameraComponent.h"
#include "AI/SCGoalActor.h"
#include "Player/SCPlayerController.h"
#include "AI/SCAICharacter.h"
#include "AI/SCAIController.h"
#include "Components/UnitTestCameraWidgetComponent.h"
#include "Skills/MainSkillsActor.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "UObject/UObjectGlobals.h"

ASC_UnitTestMainCamera::ASC_UnitTestMainCamera(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UUnitTestCameraWidgetComponent>("MainCameraWidgetComponent"))
{ }

void ASC_UnitTestMainCamera::BeginPlay()
{
	Super::BeginPlay();

	if (auto PlayerController = Cast<APlayerController>(Controller))
	{
		InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (InputSubsystem)
		{
			InputSubsystem->ClearAllMappings();
			InputSubsystem->AddMappingContext(InGameMenuMappingContext, 0);
		}
	}

	StartViewTarget = GetWorld()->SpawnActor<AActor>(StartViewTargetClass.Get(), GetActorTransform());
	StartViewTarget->SetActorRotation(FRotator(StartViewTarget->GetActorRotation().Pitch, 90, StartViewTarget->GetActorRotation().Roll));

	auto SCGameMode = Cast<ASCUnitTestGameMode>(GetWorld()->GetAuthGameMode());
	if (!SCGameMode) return;

	SCGameMode->OnGameStateChange.AddUObject(this, &ASC_UnitTestMainCamera::OnGameStateChange);
}

void ASC_UnitTestMainCamera::SetupInGameMenuInputComponentBinding(UInputComponent* PlayerInputComponent)
{
	InputSubsystem->ClearAllMappings();
	InputSubsystem->AddMappingContext(InGameMenuMappingContext, 0);
}

void ASC_UnitTestMainCamera::SetupGameInputComponentBinding(UInputComponent* PlayerInputComponent)
{
	InputSubsystem->ClearAllMappings();
	InputSubsystem->AddMappingContext(GameMappingContext, 0);
}

void ASC_UnitTestMainCamera::OnGameStateChange(EGameState NewGameState)
{
	if (NewGameState == EGameState::GAME)
	{
		IsMoveOnMouseAvailable = true;

		auto PlayerController = Cast<ASCPlayerController>(GetOwner());
		if (!PlayerController) return;

		PlayerController->SetViewTargetWithBlend(StartViewTarget, 1.0f, VTBlend_Cubic);
		DisableInput(PlayerController);
		GetWorld()->GetTimerManager().SetTimer(SetViewTargetWithBlendTimer, this, &ASC_UnitTestMainCamera::OnSetViewTargetWithBlendFinish, 1.1f, false);
	}
	else if (NewGameState == EGameState::INGAME_MENU)
	{
		IsMoveOnMouseAvailable = false;

		SelectedUnits.Empty();
		OnClearSelectUnits.Broadcast();
		SetupInGameMenuInputComponentBinding(InputComponent);
	}
}

void ASC_UnitTestMainCamera::OnSetViewTargetWithBlendFinish()
{
	auto PlayerController = Cast<ASCPlayerController>(GetOwner());
	if (!PlayerController) return;

	SetActorLocation(StartViewTarget->GetActorLocation());
	PlayerController->SetViewTargetWithBlend(this, 1.0f, VTBlend_Cubic);
	EnableInput(PlayerController);

	SetupGameInputComponentBinding(InputComponent);
}

void ASC_UnitTestMainCamera::ChangeUnitSkillsKeys()
{
	if (Cast<ASCUnitTestGameMode>(GetWorld()->GetAuthGameMode())->GetGameState() != EGameState::GAME) return;

	ASC_MainCamera::ChangeUnitSkillsKeys();
}