// Fill out your copyright notice in the Description page of Project Settings.


#include "SCGameModeBase.h"
#include "Player/SCPlayerController.h"
#include "Player/SC_MainCamera.h"

ASCGameModeBase::ASCGameModeBase()
{
	DefaultPawnClass = ASC_MainCamera::StaticClass();
	PlayerControllerClass = ASCPlayerController::StaticClass();
}