// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SCStaticMeshWeapon.h"

ASCStaticMeshWeapon::ASCStaticMeshWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	SetRootComponent(StaticMesh);
}

