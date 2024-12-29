// Fill out your copyright notice in the Description page of Project Settings.


#include "FogOfWar/FogOfWar.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AI/SCAICharacter.h"

AFogOfWar::AFogOfWar()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	SetRootComponent(StaticMeshComponent);
}

void AFogOfWar::BeginPlay()
{
	Super::BeginPlay();
	FogOfWarTraceMatDynamic = UMaterialInstanceDynamic::Create(FogOfWarTraceMat, GetWorld());
	FogOfWarMainCircleMatDynamic = UMaterialInstanceDynamic::Create(FogOfWarMainCircleMat, GetWorld());
	//FogOfWarTraceMatDynamic = CreateAndSetMaterialInstanceDynamicFromMaterial(0, FogOfWarTraceMat);
}

void AFogOfWar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	for (int32 i = 0; i < AllFriendUnitsClasses.Num(); ++i)
	{
		TArray<AActor*> AllUnitsOnScene;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AllFriendUnitsClasses[i].Get(), AllUnitsOnScene);
		
		for (int32 j = 0; j < AllUnitsOnScene.Num(); ++j)
		{
			if (!Cast<ASCAICharacter>(AllUnitsOnScene[j])->IsFriendly()) continue;
			
			DrawTraceOnFogOfWar(AllUnitsOnScene[j]->GetActorLocation());
		}
	}
}