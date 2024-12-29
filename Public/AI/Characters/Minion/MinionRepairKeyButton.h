// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MinionRepairKeyButton.generated.h"

class UWidgetComponent;

UCLASS()
class STARCRAFT_API AMinionRepairKeyButton : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UWidgetComponent* WidgetComponent;

private:
	void OnRepairKeyButtonComponentVisibility(ESlateVisibility InVisibility);
	void OnMinionEnableState();

protected:
	virtual void BeginPlay() override;

public:	
	AMinionRepairKeyButton();

	virtual void Tick(float DeltaTime) override;
	virtual void SetOwner(AActor* NewOwner) override;
};
