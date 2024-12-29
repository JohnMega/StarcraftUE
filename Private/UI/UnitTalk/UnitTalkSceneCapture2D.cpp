// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UnitTalk/UnitTalkSceneCapture2D.h"
#include "Player/SC_MainCamera.h"
#include "AI/SCAICharacter.h"
#include "AI/SCAIController.h"

void AUnitTalkSceneCapture2D::BeginPlay()
{
	Super::BeginPlay();

	if (!GetWorld()) return;

	auto PlayerController = *GetWorld()->GetPlayerControllerIterator();
	if (!PlayerController.Get()) return;

	MainCamera = Cast<ASC_MainCamera>(PlayerController->GetPawn());
	if (!MainCamera) return;

	MainCamera->OnSelectUnitsTalk.AddUObject(this, &AUnitTalkSceneCapture2D::OnSelectUnitsTalk);
}

void AUnitTalkSceneCapture2D::OnSelectUnitsTalk()
{
	if (!MainCamera) return;

	if (MainCamera->GetSelectedUnits()->Num())
	{
		auto Character = (*MainCamera->GetSelectedUnits())[0];
		//auto Controller = Cast<ASCAIController>(Character->GetOwner());
		//if (!Controller) return;

		auto CameraLocationActor = CameraLocations.Find(Character->GetCharacterClass());
		if (CameraLocationActor)
		{
			SetActorLocation((*CameraLocationActor)->GetActorLocation());
		}
	}
}