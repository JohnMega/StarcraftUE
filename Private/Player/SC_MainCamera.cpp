// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SC_MainCamera.h"
#include "Kismet/GameplayStatics.h"
#include "SCGameModeBase.h"
#include "Math/MathFwd.h"
#include "Camera/CameraComponent.h"
#include "AI/SCGoalActor.h"
#include "Player/SCPlayerController.h"
#include "AI/SCAICharacter.h"
#include "AI/Characters/Marine/SCMarine.h"
#include "AI/SCAIController.h"
#include "Components/CameraZoomComponent.h"
#include "Components/MainCameraMovementComponent.h"
#include "Components/HealthComponent.h"
#include "Components/MainCameraWidgetComponent.h"
#include "Components/SphereComponent.h"
#include "UI/SelectBoxWB.h"
#include "UI/GameMenu/GameMenuWB.h"
#include "Skills/MainSkillsActor.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "NiagaraComponent.h"
#include "Player/CameraMoveZone.h"
#include "PlotBomb/PlotBomb.h"

ASC_MainCamera::ASC_MainCamera(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(GetRootComponent());
	
	CameraDynamicMaskNiagara = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");
	CameraDynamicMaskNiagara->SetupAttachment(CameraComponent);

	CameraZoomComponent = CreateDefaultSubobject<UCameraZoomComponent>("CameraZoomComponent");

	MovementComponent = CreateDefaultSubobject<UMainCameraMovementComponent>("MovementComponent");

	MainCameraWidgetComponent = CreateDefaultSubobject<UMainCameraWidgetComponent>("MainCameraWidgetComponent");
}

void ASC_MainCamera::BeginPlay()
{
	Super::BeginPlay();

	if (auto PlayerController = Cast<APlayerController>(Controller))
	{
		InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (InputSubsystem)
		{
			InputSubsystem->AddMappingContext(GameMappingContext, 0);
		}
	}

	OnSelectUnitsTalk.AddUObject(this, &ASC_MainCamera::ChangeUnitSkillsKeys);
}

void ASC_MainCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovementComponent->SetNewActorLocation(DeltaTime, IsMoveOnMouseAvailable);
	CameraZoomComponent->CameraZoomImplementation(DeltaTime);

	for (int32 i = 0; i < SelectedUnits.Num(); ++i)
	{
		if (!IsValid(SelectedUnits[i]))
		{
			SelectedUnits.RemoveAt(i);
		}
	}

	LineTraceFromCursorForPawn();
}

bool ASC_MainCamera::IsSomeoneUnitSelected() const
{
	return !!SelectedUnits.Num();
}

const TArray<ASCAICharacter*>* ASC_MainCamera::GetSelectedUnits() const
{
	return &SelectedUnits;
}

void ASC_MainCamera::LineTraceFromCursorForPawn()
{
	if (!GetWorld()) return;

	auto PlayerController = Cast<ASCPlayerController>(GetOwner());
	if (!PlayerController) return;
	
	PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Camera), false, CursorHitResult);

	if (CursorHitResult.bBlockingHit)
	{
		auto Character = Cast<ASCAICharacter>(CursorHitResult.GetActor());
		if (Character && !Character->IsDead() && !AimedUnits.Contains(Character))
		{
			Character->ActivateNiagaraCharacterCircle(Character);
			if(bIsInSelectBoxMode) AimedUnits.Add(Character);
		}
		else if (!bIsInSelectBoxMode)
		{
			OnFriendlyCharacterAim.Broadcast(nullptr);
		}
	}
	else
	{
		OnFriendlyCharacterAim.Broadcast(nullptr);
	}
}

EMainCameraStates ASC_MainCamera::GetCurrentState() const
{
	return CurrentState;
}

void ASC_MainCamera::ChangeUnitSkillsKeys()
{
	auto FirstSelectedUnitClass = SelectedUnits[0]->GetCharacterClass();
	auto CurrentUnitSkillsMapContext = *UnitSkillsMappingContextMap.Find(FirstSelectedUnitClass);
	if (CurrentUnitSkillsMapContext)
	{
		InputSubsystem->ClearAllMappings();
		InputSubsystem->AddMappingContext(CurrentUnitSkillsMapContext, 0);
	}
}

const ASCAICharacter* ASC_MainCamera::GetFirstSelectedUnit() const
{
	return SelectedUnits.Num() > 0 ? SelectedUnits[0] : nullptr;
}

void ASC_MainCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	Input->BindAction(IACameraMove, ETriggerEvent::Triggered, this, &ASC_MainCamera::OnCameraMove);
	Input->BindAction(IACameraMove, ETriggerEvent::Completed, this, &ASC_MainCamera::OnCameraMove);

	Input->BindAction(IASelectOneUnit, ETriggerEvent::Triggered, this, &ASC_MainCamera::OnSelectOneUnitPressed);
	Input->BindAction(IASelectUnitsFinish, ETriggerEvent::Triggered, this, &ASC_MainCamera::OnSelectOneUnitReleased);

	Input->BindAction(IASelectBox, ETriggerEvent::Triggered, this, &ASC_MainCamera::OnSelectBox);
	Input->BindAction(IASelectBox, ETriggerEvent::Completed, this, &ASC_MainCamera::OnSelectBox);

	Input->BindAction(IASelectSeveralUnits, ETriggerEvent::Triggered, this, &ASC_MainCamera::OnSelectSeveralUnitsPressed);
	Input->BindAction(IASelectSeveralUnits, ETriggerEvent::Completed, this, &ASC_MainCamera::OnSelectSeveralUnitsReleased);

	Input->BindAction(IACreateGoal, ETriggerEvent::Triggered, this, &ASC_MainCamera::OnCreateGoal);
	Input->BindAction(IAAttackState, ETriggerEvent::Triggered, this, &ASC_MainCamera::OnAttackState);
	Input->BindAction(IACameraZoom, ETriggerEvent::Triggered, this, &ASC_MainCamera::CameraZoom);
	Input->BindAction(IACameraDecrease, ETriggerEvent::Triggered, this, &ASC_MainCamera::CameraDecrease);
	Input->BindAction(IAGameMenuEnable, ETriggerEvent::Triggered, this, &ASC_MainCamera::OnGameMenuEnable);
	Input->BindAction(IABombActivate, ETriggerEvent::Triggered, this, &ASC_MainCamera::OnBombActivateAction);

	Input->BindAction(IAUseStimpack, ETriggerEvent::Triggered, this, &ASC_MainCamera::OnUseStimpack);
	Input->BindAction(IAUseHealthKit, ETriggerEvent::Triggered, this, &ASC_MainCamera::OnUseHealthKit);
	Input->BindAction(IAMinionRepair, ETriggerEvent::Triggered, this, &ASC_MainCamera::OnUseMinionRepair);
}

void ASC_MainCamera::OnBombActivateAction(const FInputActionValue& Value)
{
	OnBombActivate.Broadcast();
}

void ASC_MainCamera::OnGameMenuEnable(const FInputActionValue& Value)
{
	auto GameMenuWB = MainCameraWidgetComponent->GetWidget<UGameMenuWB>();
	GameMenuWB->OnGameMenuEnable();
}

void ASC_MainCamera::OnUseStimpack(const FInputActionValue& Value)
{
	for (int32 i = 0; i < SelectedUnits.Num(); ++i)
		OnSkillUse.Broadcast(SelectedUnits[i], StimpackSkill.Get());
}

void ASC_MainCamera::OnUseHealthKit(const FInputActionValue& Value)
{
	for (int32 i = 0; i < SelectedUnits.Num(); ++i)
		OnSkillUse.Broadcast(SelectedUnits[i], HealthKitSkill.Get());
}

void ASC_MainCamera::OnUseMinionRepair(const FInputActionValue& Value)
{
	if (SelectedUnits.Num() > 0 && Cast<ASCMarine>(SelectedUnits[0]))
	{
		OnSkillUse.Broadcast(SelectedUnits[0], MinionRepairSkill.Get());
	}
}

void ASC_MainCamera::OnCameraMove(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();
	FVector2D ResultMoveVector = {0, 0};
	if (MoveVector.X)
	{
		ResultMoveVector += MoveVector.X == 1 ? (FVector2D)GetActorForwardVector() : (FVector2D)GetActorForwardVector() * (-1);
	}

	if (MoveVector.Y)
	{
		ResultMoveVector += MoveVector.Y == 1 ? (FVector2D)GetActorRightVector() : (FVector2D)GetActorRightVector() * (-1);
	}

	auto CameraMoveZone = Cast<ACameraMoveZone>(UGameplayStatics::GetActorOfClass(GetWorld(), CameraMoveZoneClass));

	FVector ResultCameraLocation = { (GetActorLocation() - CameraMoveZone->GetActorLocation()).X, (GetActorLocation() - CameraMoveZone->GetActorLocation()).Y, 0 };
	ResultCameraLocation = ResultCameraLocation.RotateAngleAxis(-GetActorRotation().Yaw, {0,0,1});

	if (ResultCameraLocation.Y > CameraMoveZone->GetMoveZoneDotPlusY().Y && MoveVector.Y == 1)
	{
		ResultMoveVector -= (FVector2D)GetActorRightVector();
	}
	
	if (ResultCameraLocation.Y < CameraMoveZone->GetMoveZoneDotMinusY().Y && MoveVector.Y == -1)
	{
		ResultMoveVector -= (FVector2D)GetActorRightVector() * (-1);
	}

	if (ResultCameraLocation.X > CameraMoveZone->GetMoveZoneDotPlusX().X && MoveVector.X == 1)
	{
		ResultMoveVector -= (FVector2D)GetActorForwardVector();
	}

	if (ResultCameraLocation.X < CameraMoveZone->GetMoveZoneDotMinusX().X && MoveVector.X == -1)
	{
		ResultMoveVector -= (FVector2D)GetActorForwardVector() * (-1);
	}

	MovementComponent->OnMove({ ResultMoveVector.X, ResultMoveVector.Y, 0});
}

void ASC_MainCamera::OnSelectBox(const FInputActionValue& Value)
{
	bool IsSelectBoxActive = Value.Get<bool>();
	if (IsSelectBoxActive)
	{
		auto PlayerController = Cast<ASCPlayerController>(GetOwner());
		if (!PlayerController) return;

		USelectBoxWB* SelectBoxWB = MainCameraWidgetComponent->GetWidget<USelectBoxWB>();

		FVector2D EndBoxCoord;
		PlayerController->GetMousePosition(EndBoxCoord.X, EndBoxCoord.Y);
		EndBoxCoord.X = SelectBoxWB->GetBoxCoords()[0].X > EndBoxCoord.X ? -EndBoxCoord.X : EndBoxCoord.X;
		EndBoxCoord.Y = SelectBoxWB->GetBoxCoords()[0].Y > EndBoxCoord.Y ? -EndBoxCoord.Y : EndBoxCoord.Y;
		SelectBoxWB->SetBoxEndCoord(EndBoxCoord);

		FVector2D StartBoxCoord = SelectBoxWB->GetBoxCoords()[0];
		if ((EndBoxCoord.Y < 0 && EndBoxCoord.X > 0) || (EndBoxCoord.Y > 0 && EndBoxCoord.X < 0))
		{
			SelectBoxWB->SetBoxCoords()[2] = { FMath::Min(StartBoxCoord.X, abs(EndBoxCoord.X)), FMath::Min(StartBoxCoord.Y, abs(EndBoxCoord.Y)) };
			SelectBoxWB->SetBoxCoords()[3] = { FMath::Max(StartBoxCoord.X, abs(EndBoxCoord.X)), FMath::Max(StartBoxCoord.Y, abs(EndBoxCoord.Y)) };
		}
		else
		{
			SelectBoxWB->SetBoxCoords()[2] = { FMath::Min(StartBoxCoord.X, abs(EndBoxCoord.X)), FMath::Max(StartBoxCoord.Y, abs(EndBoxCoord.Y)) };
			SelectBoxWB->SetBoxCoords()[3] = { FMath::Max(StartBoxCoord.X, abs(EndBoxCoord.X)), FMath::Min(StartBoxCoord.Y, abs(EndBoxCoord.Y)) };
		}

		int32 RectangelVerticesNum = 4;
		FVector2D* BoxPoints = new FVector2D[RectangelVerticesNum];
		FHitResult TempHitResult;
		for (int32 i = 0; i < RectangelVerticesNum; ++i)
		{
			PlayerController->GetHitResultAtScreenPosition({ abs(SelectBoxWB->GetBoxCoords()[i].X), abs(SelectBoxWB->GetBoxCoords()[i].Y) }, ECollisionChannel::ECC_WorldStatic, false, TempHitResult);
			*(BoxPoints + i) = { TempHitResult.ImpactPoint.X, TempHitResult.ImpactPoint.Y };
		}
		FBox2D TestBox(BoxPoints, RectangelVerticesNum);

		TArray<AActor*> AllCharacters;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASCAICharacter::StaticClass(), AllCharacters);

		if(!bIsInSelectSeveralUnitsMode) AimedUnits.Empty();
		for (int32 i = 0; i < AllCharacters.Num(); ++i)
		{
			auto Character = Cast<ASCAICharacter>(AllCharacters[i]);
			if (!Character) continue;
			if (!TestBox.IsInside({ Character->GetActorLocation().X, Character->GetActorLocation().Y }) || Character->IsDead())
			{
				Character->ActivateNiagaraCharacterCircle(nullptr);
				continue;
			}

			if (AimedUnits.Contains(Character)) continue;

			AimedUnits.Add(Character);
			Character->ActivateNiagaraCharacterCircle(Character);
		}

		delete[] BoxPoints;
	}
}

void ASC_MainCamera::OnSelectSeveralUnitsPressed(const FInputActionValue& Value)
{
	bIsInSelectSeveralUnitsMode = true;
}

void ASC_MainCamera::OnSelectSeveralUnitsReleased(const FInputActionValue& Value)
{
	bIsInSelectSeveralUnitsMode = false;
}

void ASC_MainCamera::OnSelectOneUnitReleased(const FInputActionValue& Value)
{
	int32 LastSelectedUnitsNum = SelectedUnits.Num();
	if (!bIsInSelectSeveralUnitsMode)
	{
		OnFriendlyCharacterClicked.Broadcast(nullptr);
		SelectedUnits.Empty();
	}

	MainCameraWidgetComponent->GetWidget<USelectBoxWB>()->ClearBoxCoords();

	for (int32 i = 0; i < AimedUnits.Num(); ++i)
	{
		if (AimedUnits[i]->IsFriendly() && !SelectedUnits.Contains(AimedUnits[i]))
		{
			SelectedUnits.Add(AimedUnits[i]);
			OnFriendlyCharacterClicked.Broadcast(AimedUnits[i]);
		}
	}

	if(!bIsInSelectSeveralUnitsMode) AimedUnits.Empty();
	bIsInSelectBoxMode = false;

	if (SelectedUnits.Num())
	{
		OnSelectUnitsTalk.Broadcast();
	}

	if (!LastSelectedUnitsNum && SelectedUnits.Num())
	{
		OnSelectUnits.Broadcast();
	}
	else if (LastSelectedUnitsNum && !SelectedUnits.Num())
	{
		OnClearSelectUnits.Broadcast();
	}
}

void ASC_MainCamera::OnCreateGoal(const FInputActionValue& Value)
{
	if (!GetWorld() || !SelectedUnits.Num()) return;
	
	ASCPlayerController* PlayerController = Cast<ASCPlayerController>(GetOwner());
	if (!PlayerController) return;

	PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Camera), false, CursorHitResult);

	if (CursorHitResult.bBlockingHit)
	{
		for (int32 i = 0; i < SelectedUnits.Num(); ++i)
		{
			if (SelectedUnits[i]->IsDead()) continue;

			FVector GoalActorLocation = { CursorHitResult.ImpactPoint.X, CursorHitResult.ImpactPoint.Y, CursorHitResult.ImpactPoint.Z + 0.1F };
			auto GoalActor = GetWorld()->SpawnActor<ASCGoalActor>(GoalActorClass.Get(), FTransform(FRotator::ZeroRotator, GoalActorLocation));

			if (GoalActor)
			{
				if (CurrentState == EMainCameraStates::DEFAULT)
				{
					auto Character = Cast<ASCAICharacter>(CursorHitResult.GetActor());
					if (Character)
					{
						GoalActor->SetActorLocation(Character->GetMesh()->GetSocketLocation(CharacterInGoalSpawnDotName));

						if (!Character->IsFriendly())
						{
							SelectedUnits[i]->SetAttackTargetCharacter(Character);
							SelectedUnits[i]->SetCurrentGoal(GoalActor);
							SelectedUnits[i]->OnAddNewGoalActor(static_cast<EAICharacterState>(EMainCameraStates::INDIVIDUAL_ATTACK));
							continue;
						}
					}

					SelectedUnits[i]->SetCurrentGoal(GoalActor);
					SelectedUnits[i]->OnAddNewGoalActor(static_cast<EAICharacterState>(CurrentState));
				}
				else if (CurrentState == EMainCameraStates::ATTACK)
				{
					auto Character = Cast<ASCAICharacter>(CursorHitResult.GetActor());
					if (Character)
					{
						GoalActor->SetActorLocation(Character->GetMesh()->GetSocketLocation(CharacterInGoalSpawnDotName));
						if (SelectedUnits[i] != Character)
						{
							SelectedUnits[i]->SetAttackTargetCharacter(Character);
							SelectedUnits[i]->SetCurrentGoal(GoalActor);
							SelectedUnits[i]->OnAddNewGoalActor(static_cast<EAICharacterState>(EMainCameraStates::INDIVIDUAL_ATTACK));
						}
						else
						{
							GoalActor->Destroy();
						}
					}
					else
					{
						SelectedUnits[i]->SetCurrentGoal(GoalActor);
						SelectedUnits[i]->OnAddNewGoalActor(static_cast<EAICharacterState>(CurrentState));
					}
				}

				
			}
		}
	}

	CurrentState = EMainCameraStates::DEFAULT;
	OnStateChange.Broadcast(CurrentState);
}

void ASC_MainCamera::CameraZoom(const FInputActionValue& Value)
{
	CameraZoomComponent->OnCameraZoom();
}

void ASC_MainCamera::CameraDecrease(const FInputActionValue& Value)
{
	CameraZoomComponent->OnCameraDecrease();
}

void ASC_MainCamera::OnSelectOneUnitPressed(const FInputActionValue& Value)
{
	auto PlayerController = Cast<ASCPlayerController>(GetOwner());
	if (!PlayerController) return;

	FVector2D CursorCoords;
	PlayerController->GetMousePosition(CursorCoords.X, CursorCoords.Y);
	MainCameraWidgetComponent->GetWidget<USelectBoxWB>()->SetBoxStartCoord(CursorCoords);

	bIsInSelectBoxMode = true;
}

void ASC_MainCamera::OnAttackState(const FInputActionValue& Value)
{
	CurrentState = EMainCameraStates::ATTACK;
	OnStateChange.Broadcast(CurrentState);
}