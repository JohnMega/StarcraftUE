// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UnitSkillsWB.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/Overlay.h"
#include "Animation/WidgetAnimation.h"
#include "Player/SC_MainCamera.h"
#include "AI/SCAIController.h"
#include "AI/SCAICharacter.h"

void UUnitSkillsWB::BeginPlay()
{
	if (!GetWorld()) return;

	auto PlayerController = *GetWorld()->GetPlayerControllerIterator();
	if (!PlayerController.Get()) return;

	MainCamera = Cast<ASC_MainCamera>(PlayerController->GetPawn());
	if (!MainCamera) return;

	MainCamera->OnClearSelectUnits.AddUObject(this, &UUnitSkillsWB::OnClearSelectUnits);
	MainCamera->OnSelectUnitsTalk.AddUObject(this, &UUnitSkillsWB::OnSelectUnitsTalk);

	auto AllUnitsSkillsBoxesArray = AllUnitsSkillsBoxes->GetAllChildren();
	for (int32 i = 0; i < UnitsClasses.Num(); ++i)
	{
		UnitsSkillsBoxesMap.Add(UnitsClasses[i], Cast<UHorizontalBox>(AllUnitsSkillsBoxesArray[i]));
	}
}

void UUnitSkillsWB::OnClearSelectUnits()
{
	if (OpenSkillsMenuLastAnimType == ELastAnimationTypePlayed::DEFAULT)
	{
		PlayAnimationReverse(OpenSkillsMenu);
		OpenSkillsMenuLastAnimType = ELastAnimationTypePlayed::REVERSE;
	}
}

void UUnitSkillsWB::OnSelectUnitsTalk()
{
	if (OpenSkillsMenuLastAnimType == ELastAnimationTypePlayed::REVERSE)
	{
		PlayAnimation(OpenSkillsMenu);
		OpenSkillsMenuLastAnimType = ELastAnimationTypePlayed::DEFAULT;

		ReplaceUnitSkillsHorBox();
	}
	else if (OpenSkillsMenuLastAnimType == ELastAnimationTypePlayed::DEFAULT)
	{
		ReplaceUnitSkillsHorBox();
	}
}

void UUnitSkillsWB::ReplaceUnitSkillsHorBox()
{
	auto FirstSelectedUnit = (*MainCamera->GetSelectedUnits())[0];
	//auto Controller = Cast<ASCAIController>(FirstSelectedUnit->GetOwner());

	UHorizontalBox* UnitSkillBox = Cast<UHorizontalBox>(*UnitsSkillsBoxesMap.Find(FirstSelectedUnit->GetCharacterClass()));
	if (UnitSkillBox)
	{
		VisibleUnitSkillsHorBox->ClearChildren();
		auto UnitSkillBoxChilds = UnitSkillBox->GetAllChildren();

		for (int32 i = 0; i < UnitSkillBoxChilds.Num(); ++i)
		{
			auto Child = DuplicateObject<UWidget>(UnitSkillBoxChilds[i], this);
			VisibleUnitSkillsHorBox->AddChild(Child);
			OnVisibleUnitSkillsBoxAddChild.Broadcast(Child);
		}
	}
}