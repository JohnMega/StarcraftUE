// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"
#include "AI/SCAICharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/HealthBarWB.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeAnyDamage);
}

void UHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (IsDead()) return;

	Health = FMath::Clamp(Health - Damage, 0, MaxHealth);
	SetWidgetHealthPercent();

	if (Health <= 0.0f)
	{
		CharacterDeath();
	}
}

void UHealthComponent::SetWidgetHealthPercent() const
{
	auto WidgetComponent = GetOwner()->GetComponentByClass<UWidgetComponent>();
	if (!WidgetComponent) return;

	auto WBPHealthBar = Cast<UHealthBarWB>(WidgetComponent->GetUserWidgetObject());
	if (!WBPHealthBar) return;

	WBPHealthBar->SetHealthPercent(Health / MaxHealth);
}

void UHealthComponent::CharacterDeath()
{
	if (GetOwner()->GetWorldTimerManager().IsTimerActive(CharacterDestroyTimer)) return;

	OnCharacterDeath.Broadcast();
	GetOwner()->GetWorldTimerManager().SetTimer(CharacterDestroyTimer, this, &UHealthComponent::DestroyCharacter, 5.0f, false);
}

void UHealthComponent::DestroyCharacter()
{
	auto Character = Cast<ASCAICharacter>(GetOwner());
	Character->DestroySCCharacter();
}

float UHealthComponent::GetDamage() const
{
	return DamageValue;
}

float UHealthComponent::GetCurrentHealth() const
{
	return Health;
}

bool UHealthComponent::IsDead() const
{
	return (Health <= 0.0f);
}