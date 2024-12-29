// Fill out your copyright notice in the Description page of Project Settings.


#include "Triggers/OneShotTrigger.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "AI/SCAICharacter.h"

AOneShotTrigger::AOneShotTrigger()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	SetRootComponent(BoxComponent);
}

void AOneShotTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &AOneShotTrigger::OnTriggerBeginOverlap);
}

void AOneShotTrigger::OnTriggerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	TArray<AActor*> AllUnits;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASCAICharacter::StaticClass(), AllUnits);

	for (int i = 0; i < AllUnits.Num(); ++i)
	{
		if (OnlyFriendUnitCanActivate && !Cast<ASCAICharacter>(AllUnits[i])->IsFriendly()) continue;

		if (OtherActor == AllUnits[i])
		{
			OnOneShotTriggerActivate.Broadcast();
			Destroy();
			break;
		}
	}
}