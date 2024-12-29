// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OneShotTrigger.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOneShotTriggerActivateSignature);

class UBoxComponent;

UCLASS()
class STARCRAFT_API AOneShotTrigger : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditInstanceOnly, Category = "Trigger Activate")
	bool OnlyFriendUnitCanActivate = false;

	//UPROPERTY(EditInstanceOnly, Category = "Trigger Activate")
	//TArray<AActor*> TriggerActivateActors;

public:
	UPROPERTY(BlueprintAssignable)
	FOnOneShotTriggerActivateSignature OnOneShotTriggerActivate;

private:
	UFUNCTION()
	void OnTriggerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

public:	
	AOneShotTrigger();

protected:
	virtual void BeginPlay() override;
};
