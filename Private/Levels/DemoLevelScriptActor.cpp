// Fill out your copyright notice in the Description page of Project Settings.


#include "Levels/DemoLevelScriptActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/LoadLevelScreen/LoadLevelScreenWB.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void ADemoLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	auto PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if(PlayerPawn)
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	LoadLevelScreenWB = Cast<ULoadLevelScreenWB>(CreateWidget(GetWorld(), LoadLevelScreenWBClass));
	LoadLevelScreenWB->AddToViewport(100);

	PercentValue = 0;
	PercentStep = 1.0f / SubLevelsNames.Num();
	for (int32 i = 0; i < SubLevelsNames.Num(); ++i)
	{
		FTimerHandle IterTimer;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "SubLevelLoadIteration", i);
		GetWorld()->GetTimerManager().SetTimer(IterTimer, Delegate, i + 0.1f, false);
	}

	FTimerHandle DestroyTimer;
	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &ADemoLevelScriptActor::DestroyLoadLevelScreenWB, SubLevelsNames.Num(), false);
}

void ADemoLevelScriptActor::SubLevelLoadIteration(int32 SubLevelNumber)
{
	FLatentActionInfo info;
	info.UUID = SubLevelNumber;
	UGameplayStatics::LoadStreamLevel(this, SubLevelsNames[SubLevelNumber], true, true, info);

	PercentValue += PercentStep;
	FLoadLevelIterationInfo IterInfo;
	IterInfo.PercentValue = PercentValue;
	IterInfo.CurrentSubLevelDescription = SubLevelsDescriptionNames[SubLevelNumber];
	LoadLevelScreenWB->LoadLevelNextIteration(IterInfo);
}

void ADemoLevelScriptActor::DestroyLoadLevelScreenWB()
{
	LoadLevelScreenWB->RemoveFromParent();
	auto PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	OnLoadLevelFinish.Broadcast();
}