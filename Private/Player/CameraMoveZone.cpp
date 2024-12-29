// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CameraMoveZone.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ChildActorComponent.h"

ACameraMoveZone::ACameraMoveZone()
{
	PrimaryActorTick.bCanEverTick = false;

	CameraMoveZone = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh Component");
	SetRootComponent(CameraMoveZone);

	MoveZoneDotMinusX = CreateDefaultSubobject<UChildActorComponent>("MoveZoneDotMinusX");
	MoveZoneDotMinusX->SetupAttachment(GetRootComponent());

	MoveZoneDotPlusX = CreateDefaultSubobject<UChildActorComponent>("MoveZoneDotPlusX");
	MoveZoneDotPlusX->SetupAttachment(GetRootComponent());

	MoveZoneDotMinusY = CreateDefaultSubobject<UChildActorComponent>("MoveZoneDotMinusY");
	MoveZoneDotMinusY->SetupAttachment(GetRootComponent());

	MoveZoneDotPlusY = CreateDefaultSubobject<UChildActorComponent>("MoveZoneDotPlusY");
	MoveZoneDotPlusY->SetupAttachment(GetRootComponent());
}

FVector ACameraMoveZone::GetMoveZoneDotMinusX() const
{
	/*FVector XAxis = { 1,0,0 };
	FVector YAxis = { 0,1,0 };
	FVector ZAxis = { 0,0,1 };
	FVector NewComponentLocation = MoveZoneDotMinusX->GetComponentLocation() - GetActorLocation();
	FVector ResultComponentLocation =
	{
		XAxis.X * NewComponentLocation.X + YAxis.X * NewComponentLocation.Y + ZAxis.X * NewComponentLocation.Z,
		XAxis.Y * NewComponentLocation.X + YAxis.Y * NewComponentLocation.Y + ZAxis.Y * NewComponentLocation.Z,
		XAxis.Z * NewComponentLocation.X + YAxis.Z * NewComponentLocation.Y + ZAxis.Z * NewComponentLocation.Z
	};*/

	return MoveZoneDotMinusX->GetComponentLocation() - GetActorLocation();
}

FVector ACameraMoveZone::GetMoveZoneDotPlusX() const
{
	/*FVector XAxis = { 1,0,0 };
	FVector YAxis = { 0,1,0 };
	FVector ZAxis = { 0,0,1 };
	FVector NewComponentLocation = MoveZoneDotPlusX->GetComponentLocation() - GetActorLocation();
	FVector ResultComponentLocation =
	{
		XAxis.X * NewComponentLocation.X + YAxis.X * NewComponentLocation.Y + ZAxis.X * NewComponentLocation.Z,
		XAxis.Y * NewComponentLocation.X + YAxis.Y * NewComponentLocation.Y + ZAxis.Y * NewComponentLocation.Z,
		XAxis.Z * NewComponentLocation.X + YAxis.Z * NewComponentLocation.Y + ZAxis.Z * NewComponentLocation.Z
	};*/

	return MoveZoneDotPlusX->GetComponentLocation() - GetActorLocation();
}

FVector ACameraMoveZone::GetMoveZoneDotMinusY() const
{
	/*FVector XAxis = { 1,0,0 };
	FVector YAxis = { 0,1,0 };
	FVector ZAxis = { 0,0,1 };
	FVector NewComponentLocation = MoveZoneDotMinusY->GetComponentLocation() - GetActorLocation();
	FVector ResultComponentLocation =
	{
		XAxis.X * NewComponentLocation.X + YAxis.X * NewComponentLocation.Y + ZAxis.X * NewComponentLocation.Z,
		XAxis.Y * NewComponentLocation.X + YAxis.Y * NewComponentLocation.Y + ZAxis.Y * NewComponentLocation.Z,
		XAxis.Z * NewComponentLocation.X + YAxis.Z * NewComponentLocation.Y + ZAxis.Z * NewComponentLocation.Z
	};*/

	return MoveZoneDotMinusY->GetComponentLocation() - GetActorLocation();
}

FVector ACameraMoveZone::GetMoveZoneDotPlusY() const
{
	/*FVector XAxis = { 1,0,0 };
	FVector YAxis = { 0,1,0 };
	FVector ZAxis = { 0,0,1 };
	FVector NewComponentLocation = MoveZoneDotPlusY->GetComponentLocation() - GetActorLocation();
	FVector ResultComponentLocation =
	{
		XAxis.X * NewComponentLocation.X + YAxis.X * NewComponentLocation.Y + ZAxis.X * NewComponentLocation.Z,
		XAxis.Y * NewComponentLocation.X + YAxis.Y * NewComponentLocation.Y + ZAxis.Y * NewComponentLocation.Z,
		XAxis.Z * NewComponentLocation.X + YAxis.Z * NewComponentLocation.Y + ZAxis.Z * NewComponentLocation.Z
	};*/

	return MoveZoneDotPlusY->GetComponentLocation() - GetActorLocation();
}

FVector ACameraMoveZone::GetUpYVector() const
{
	return FVector(MoveZoneDotPlusY->GetComponentLocation() - GetActorLocation()).GetSafeNormal();
}

FVector ACameraMoveZone::GetUpXVector() const
{
	return FVector(MoveZoneDotPlusX->GetComponentLocation() - GetActorLocation()).GetSafeNormal();
}

FVector ACameraMoveZone::GetUpZVector() const
{
	return FVector{0, 0, 1};
}