// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SCGoalActor.generated.h"

class ASCPlayerController;
class UNiagaraComponent;
enum class EAICharacterState : uint8;

UCLASS()
class STARCRAFT_API ASCGoalActor : public AActor
{
	GENERATED_BODY()

protected:
	ASCPlayerController* SCPlayerController;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UNiagaraComponent* NiagaraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Niagara")
	FName NiagaraColorVarName = "CursorColor";

protected:
	virtual void BeginPlay() override;

public:	
	ASCGoalActor();

	virtual void Tick(float DeltaTime) override;

	void SetNiagaraColor(EAICharacterState CharacterState);

	UFUNCTION(BlueprintCallable)
	void SetNiagaraVisible(bool Visible);
};
