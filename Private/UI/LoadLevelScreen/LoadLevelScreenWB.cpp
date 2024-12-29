// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LoadLevelScreen/LoadLevelScreenWB.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"

void ULoadLevelScreenWB::LoadLevelNextIteration(FLoadLevelIterationInfo LoadLevelIterationInfo)
{
	LoadLevelProgress->SetPercent(LoadLevelIterationInfo.PercentValue);
	LoadLevelDescriptionText->SetText(FText::FromString(LoadLevelIterationInfo.CurrentSubLevelDescription.ToString()));
}