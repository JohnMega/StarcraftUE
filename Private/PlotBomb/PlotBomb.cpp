// Fill out your copyright notice in the Description page of Project Settings.


#include "PlotBomb/PlotBomb.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ChildActorComponent.h"
#include "NiagaraSystem.h"
#include "Player/SC_MainCamera.h"
#include "AI/SCAICharacter.h"
#include "NiagaraFunctionLibrary.h"

APlotBomb::APlotBomb()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	SetRootComponent(StaticMeshComponent);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	WidgetComponent->SetupAttachment(GetRootComponent());

	BombActivateEffectSpawnLocation = CreateDefaultSubobject<UChildActorComponent>("BombActivateEffectSpawnLocation");
	BombActivateEffectSpawnLocation->SetupAttachment(GetRootComponent());
}

void APlotBomb::BeginPlay()
{
	Super::BeginPlay();

	GetMainCamera();

	for (int32 i = 0; i < StaticMeshComponent->GetNumMaterials(); ++i)
	{
		StaticMeshDefaultMaterials.Add(StaticMeshComponent->GetMaterial(i));
		StaticMeshComponent->SetMaterial(i, nullptr);
	}

	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetMaterial(0, BombDisabledMaterial);

	GetWorld()->GetTimerManager().SetTimer(ObsCharacterDistCheckTimer, this, &APlotBomb::ObsCharacterDistCheck, 0.2f, true);
}

void APlotBomb::GetMainCamera()
{
	MainCamera = Cast<ASC_MainCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), ASC_MainCamera::StaticClass()));
	MainCamera->OnBombActivate.AddUObject(this, &APlotBomb::OnBombActivate);
}

void APlotBomb::OnBombActivate()
{
	if (!bIsEnabledToActivate || bIsBombActive) return;

	bool IsObsCharacterSelected = false;
	for (int32 i = 0; i < MainCamera->GetSelectedUnits()->Num(); ++i)
	{
		if (Cast<AActor>((*MainCamera->GetSelectedUnits())[i]) == ObservedCharacter)
		{
			IsObsCharacterSelected = true;
			break;
		}
	}

	if (IsObsCharacterSelected)
	{
		for (int32 i = 0; i < StaticMeshDefaultMaterials.Num(); ++i)
		{
			StaticMeshComponent->SetMaterial(i, StaticMeshDefaultMaterials[i]);
		}
		
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BombActivateEffect, BombActivateEffectSpawnLocation->GetComponentLocation());
		GetWorld()->GetTimerManager().ClearTimer(ObsCharacterDistCheckTimer);
		WidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
		bIsBombActive = true;
	}
}

void APlotBomb::ObsCharacterDistCheck()
{
	float ObsCharacterDist = FVector(ObservedCharacter->GetActorLocation() - GetActorLocation()).Length();
	if (ObsCharacterDist < BombActivateDistance)
	{
		bIsEnabledToActivate = true;
		WidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		bIsEnabledToActivate = false;
		WidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
	}
}

bool APlotBomb::IsBombActive() const
{
	return bIsBombActive;
}