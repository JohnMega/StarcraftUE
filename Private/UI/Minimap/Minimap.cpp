// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Minimap/Minimap.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ChildActorComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Camera/CameraComponent.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AI/SCAICharacter.h"

AMinimap::AMinimap()
{
	PrimaryActorTick.bCanEverTick = true;

	MinimapPlane = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	SetRootComponent(MinimapPlane);

	PlaneCentre = CreateDefaultSubobject<UChildActorComponent>("PlaneCentre");
	PlaneCentre->SetupAttachment(GetRootComponent());

	PlaneScaleDistanceDot = CreateDefaultSubobject<UChildActorComponent>("PlaneScaleDistanceDot");
	PlaneScaleDistanceDot->SetupAttachment(GetRootComponent());

	MinimapPlaneMoveStartDot = CreateDefaultSubobject<UChildActorComponent>("MinimapPlaneMoveStartDot");
	MinimapPlaneMoveStartDot->SetupAttachment(GetRootComponent());

	MinimapCamera = CreateDefaultSubobject<USceneCaptureComponent2D>("MinimapCamera");
}

void AMinimap::BeginPlay()
{
	Super::BeginPlay();

	MinimapPlaneMID = MinimapPlane->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MinimapPlaneMat);
	MinimapFriendUnitsMaskMID = UMaterialInstanceDynamic::Create(MinimapFriendUnitsMaskMat, GetWorld());
	MinimapEnemyUnitsMaskMID = UMaterialInstanceDynamic::Create(MinimapEnemyUnitsMaskMat, GetWorld());
	MinimapCameraBoxMID = UMaterialInstanceDynamic::Create(MinimapCameraBoxMat, GetWorld());

	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), MinimapFriendUnitsMaskRT, MinimapFriendUnitsMaskRT->ClearColor);
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), MinimapEnemyUnitsMaskRT, MinimapEnemyUnitsMaskRT->ClearColor);
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), MinimapCameraBoxRT, MinimapCameraBoxRT->ClearColor);
	
	// Minimap Scale Code
	//MinimapCamera->bCaptureEveryFrame = true;
	//ScaleXActorClass = GetWorld()->SpawnActor<AActor>(ScaleActorsClass.Get(), GetActorTransform());
	//ScaleYActorClass = GetWorld()->SpawnActor<AActor>(ScaleActorsClass.Get(), GetActorTransform());

	LastBoxDots.Add({ { 0,0,0 }, "BoxDot1" });
	LastBoxDots.Add({ { 0,0,0 }, "BoxDot2" });
	LastBoxDots.Add({ { 0,0,0 }, "BoxDot3" });
	LastBoxDots.Add({ { 0,0,0 }, "BoxDot4" });
}

void AMinimap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Minimap Scale Code
	/*if (!IsMinimapPlaneScaledX || !IsMinimapPlaneScaledY)
	{
		SetMinimapPlaneScale();
		return;
	}
	else if (IsMinimapPlaneScaledX && IsMinimapPlaneScaledY && !IsMinimapPlaneFullyScaled)
	{
		OnMinimapPlaneFullyScaled();
	}*/

	if(!IsMinimapPlaneFullyScaled)
		OnMinimapPlaneFullyScaled();

	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), MinimapFriendUnitsMaskRT, MinimapFriendUnitsMaskRT->ClearColor);
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), MinimapEnemyUnitsMaskRT, MinimapEnemyUnitsMaskRT->ClearColor);
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), MinimapCameraBoxRT, MinimapCameraBoxRT->ClearColor);

	UnitsMasksHandle();
	CameraBoxHandle();
}

void AMinimap::CameraBoxHandle()
{
	auto RotationAngleHandle = [&]()
		{
			float MinimapRotationAngle; 
			MinimapPlaneMID->GetScalarParameterValue(FName("MinimapRotationAngle"), MinimapRotationAngle);

			if (MinimapRotationAngle != 0.0)
			{
				FVector TempMinimapCameraBoxSize = MinimapCameraBoxSize;
				MinimapCameraBoxSize.X = TempMinimapCameraBoxSize.Y;
				MinimapCameraBoxSize.Y = TempMinimapCameraBoxSize.X;

				IsMinimapRotationAngleInitFinish = true;
			}
		};

	if(!IsMinimapRotationAngleInitFinish) 
		RotationAngleHandle();

	FVector PlayerPawnLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	CameraBoxCoordToMinimap(MinimapCameraBoxSize + PlayerPawnLocation, LastBoxDots[0].second, LastBoxDots[0].first);
	CameraBoxCoordToMinimap(FVector{ -MinimapCameraBoxSize.X, MinimapCameraBoxSize.Y, 0 } + PlayerPawnLocation, LastBoxDots[1].second, LastBoxDots[1].first);
	CameraBoxCoordToMinimap(FVector{ -MinimapCameraBoxSize.X, -MinimapCameraBoxSize.Y, 0 } + PlayerPawnLocation, LastBoxDots[2].second, LastBoxDots[2].first);
	CameraBoxCoordToMinimap(FVector{ MinimapCameraBoxSize.X, -MinimapCameraBoxSize.Y, 0 } + PlayerPawnLocation, LastBoxDots[3].second, LastBoxDots[3].first);
	
	MoveBoxDotsToBoxCenter();
	GetCameraUVPosition(LastCameraUVPosition);

	DrawMinimapCameraBoxToRenderTarget();
}

void AMinimap::CameraBoxCoordToMinimap(FVector BoxDotLocation, FName BoxDotMatName, FLinearColor& LastBoxDotCoord)
{
	MinimapCameraBoxMID->GetVectorParameterValue(BoxDotMatName, LastBoxDotCoord);

	UVLineTraceByMinimap(BoxDotLocation + FVector{ 0, 0, 5000 }, BoxDotLocation - FVector{ 0,0, 5000 });
	FVector UVLocation = UVLineTraceByMinimapResult;
	MinimapCameraBoxMID->SetVectorParameterValue(BoxDotMatName, FVector4{ UVLocation.X, UVLocation.Y, 0, 1});

	BoxDotBeyondBorderCheck({ LastBoxDotCoord.R, LastBoxDotCoord.G, 0 }, BoxDotMatName);
}

void AMinimap::BoxDotBeyondBorderCheck(FVector LastBoxDotCoord, FName BoxDotMatName)
{
	FLinearColor CurrentLastBoxDotCoord;
	MinimapCameraBoxMID->GetVectorParameterValue(BoxDotMatName, CurrentLastBoxDotCoord);
	
	if (CurrentLastBoxDotCoord == FLinearColor{0, 0, 0})
	{
		MinimapCameraBoxMID->SetVectorParameterValue(BoxDotMatName, LastBoxDotCoord);
	}
}

void AMinimap::MoveBoxDotsToBoxCenter()
{
	if (LastCameraUVPosition == FVector{ 0,0,0 }) return;

	FVector CurrentCameraUVPosition;
	GetCameraUVPosition(CurrentCameraUVPosition);

	if (CurrentCameraUVPosition == FVector{ 0,0,0 }) return;
	for (int32 i = 0; i < LastBoxDots.Num(); ++i)
	{
		MinimapCameraBoxMID->SetVectorParameterValue(LastBoxDots[i].second, ((FVector)LastBoxDots[i].first - LastCameraUVPosition) + CurrentCameraUVPosition);
	}
}

void AMinimap::GetCameraUVPosition(FVector& CameraUVPosition)
{
	FVector WorldCameraLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	UVLineTraceByMinimap(WorldCameraLocation + FVector{ 0,0,5000 }, WorldCameraLocation - FVector{ 0,0,5000 });
	CameraUVPosition = UVLineTraceByMinimapResult;
}

void AMinimap::UnitsMasksHandle()
{
	auto IsUnitHidden = [&](AActor* CheckUnit)
		{
			int32 HiddenActorIndex;
			if (!MinimapCamera->HiddenActors.Find(CheckUnit, HiddenActorIndex))
			{
				MinimapCamera->HiddenActors.Add(CheckUnit);
			}
		};

	

	for (int32 i = 0; i < AllUnitsClasses.Num(); ++i)
	{
		TArray<AActor*> AllUnits;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AllUnitsClasses[i], AllUnits);

		for (int32 j = 0; j < AllUnits.Num(); ++j)
		{
			IsUnitHidden(AllUnits[j]);

			if (Cast<ASCAICharacter>(AllUnits[j])->IsDead()) continue;

			if (Cast<ASCAICharacter>(AllUnits[j])->IsFriendly())
				DrawFriendUnitLocationInfoToRenderTarget(AllUnits[j]->GetActorLocation());
			else
				DrawEnemyUnitLocationInfoToRenderTarget(AllUnits[j]->GetActorLocation());
		}
	}
}

void AMinimap::OnMinimapPlaneFullyScaled()
{
	MinimapCamera->bCaptureEveryFrame = false;
	IsMinimapPlaneFullyScaled = true;

	GetWorld()->GetTimerManager().SetTimer(MinimapCameraCaptureTimer, this, &AMinimap::MinimapCameraCapture, 0.2f, true);
}

void AMinimap::MinimapCameraCapture()
{
	MinimapCamera->CaptureScene();
}

void AMinimap::SetMinimapPlaneScale()
{
	MinimapPlaneScalingX();
	MinimapPlaneScalingY();
}

void AMinimap::MinimapPlaneScalingX()
{
	if(ScaleXActorClass->WasRecentlyRendered())
	{
		ScaleXActorClass->SetActorLocation(ScaleXActorClass->GetActorLocation() + FVector{ 100, 0, 0 });
	}
	else if (FVector(ScaleXActorClass->GetActorLocation() - PlaneCentre->GetComponentLocation()).Length() > 500)
	{
		float OneScaleValue = FVector(PlaneScaleDistanceDot->GetComponentLocation() - PlaneCentre->GetComponentLocation()).Length();
		float ScaleX = FVector(ScaleXActorClass->GetActorLocation() - PlaneCentre->GetComponentLocation()).Length() / OneScaleValue;
		SetActorScale3D({ ScaleX, GetActorScale3D().Y, 0.1F });

		ScaleXActorClass->Destroy();
		IsMinimapPlaneScaledX = true;
	}
}

void AMinimap::MinimapPlaneScalingY()
{
	if (ScaleYActorClass->WasRecentlyRendered())
	{
		ScaleYActorClass->SetActorLocation(ScaleYActorClass->GetActorLocation() + FVector{ 0, 100, 0 });
	}
	else if(FVector(ScaleYActorClass->GetActorLocation() - PlaneCentre->GetComponentLocation()).Length() > 500)
	{
		float OneScaleValue = FVector(PlaneScaleDistanceDot->GetComponentLocation() - PlaneCentre->GetComponentLocation()).Length();
		float ScaleY = FVector(ScaleYActorClass->GetActorLocation() - PlaneCentre->GetComponentLocation()).Length() / OneScaleValue;
		SetActorScale3D({ GetActorScale3D().X, ScaleY, 0.1F});

		ScaleYActorClass->Destroy();
		IsMinimapPlaneScaledY = true;
	}
}

float AMinimap::GetMinimapPlaneHalfLenght() const
{
	return FVector(PlaneScaleDistanceDot->GetComponentLocation() - PlaneCentre->GetComponentLocation()).Length();
}

UChildActorComponent* AMinimap::GetMinimapPlaneMoveStartDot() const
{
	return MinimapPlaneMoveStartDot;
}

UMaterialInstanceDynamic* AMinimap::GetMinimapPlaneMID() const
{
	return MinimapPlaneMID;
}

UMaterialInstanceDynamic* AMinimap::GetMinimapCameraBoxMID() const
{
	return MinimapCameraBoxMID;
}