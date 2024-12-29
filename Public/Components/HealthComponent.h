// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCharacterDeathSignature)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STARCRAFT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	FTimerHandle CharacterDestroyTimer;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float Health = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxHealth = Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float DamageValue = 5.0f;

public:
	FOnCharacterDeathSignature OnCharacterDeath;

private:
	void DestroyCharacter();
	void SetWidgetHealthPercent() const;

protected:
	virtual void BeginPlay() override;
		
public:
	UHealthComponent();

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void CharacterDeath();

	float GetDamage() const;
	float GetCurrentHealth() const;

	bool IsDead() const;
};
