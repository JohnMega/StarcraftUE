// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Minimap.generated.h"

class UStaticMeshComponent;
class UMaterialInstanceDynamic;
class UMaterialInstance;
class UTextureRenderTarget2D;
class UChildActorComponent;
class USceneCaptureComponent2D;
class UCameraComponent;

UCLASS()
class STARCRAFT_API AMinimap : public APawn
{
	GENERATED_BODY()

private:
	TArray<std::pair<FLinearColor, FName>, TInlineAllocator<4>> LastBoxDots;

	FTimerHandle MinimapCameraCaptureTimer;
	FVector LastCameraUVPosition;
	AActor* ScaleXActorClass;
	AActor* ScaleYActorClass;
	bool IsMinimapPlaneScaledX = false;
	bool IsMinimapPlaneScaledY = false;
	bool IsMinimapPlaneFullyScaled = false;
	bool IsMinimapRotationAngleInitFinish = false;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MinimapPlane;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UChildActorComponent* PlaneCentre;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UChildActorComponent* PlaneScaleDistanceDot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UChildActorComponent* MinimapPlaneMoveStartDot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneCaptureComponent2D* MinimapCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Materials")
	UMaterialInstance* MinimapPlaneMat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Materials")
	UMaterialInstance* MinimapFriendUnitsMaskMat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Materials")
	UMaterialInstance* MinimapEnemyUnitsMaskMat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Materials")
	UMaterialInstance* MinimapCameraBoxMat;

	UPROPERTY(BlueprintReadWrite, Category = "Materials")
	UMaterialInstanceDynamic* MinimapPlaneMID;

	UPROPERTY(BlueprintReadWrite, Category = "Materials")
	UMaterialInstanceDynamic* MinimapFriendUnitsMaskMID;

	UPROPERTY(BlueprintReadWrite, Category = "Materials")
	UMaterialInstanceDynamic* MinimapEnemyUnitsMaskMID;

	UPROPERTY(BlueprintReadWrite, Category = "Materials")
	UMaterialInstanceDynamic* MinimapCameraBoxMID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Render Targets")
	UTextureRenderTarget2D* MinimapFriendUnitsMaskRT;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Render Targets")
	UTextureRenderTarget2D* MinimapEnemyUnitsMaskRT;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Render Targets")
	UTextureRenderTarget2D* MinimapCameraBoxRT;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Classes")
	TArray<TSubclassOf<AActor>> AllUnitsClasses;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Classes")
	TSubclassOf<AActor> ScaleActorsClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sizes")
	FVector MinimapCameraBoxSize = { 500, 250, 0 };

	UPROPERTY(BlueprintReadWrite, Category = "Line Trace")
	FVector UVLineTraceByMinimapResult;

private:
	void OnMinimapPlaneFullyScaled();
	void MinimapCameraCapture();
	void SetMinimapPlaneScale();
	void MinimapPlaneScalingX();
	void MinimapPlaneScalingY();

	void UnitsMasksHandle();

	void CameraBoxHandle();
	void CameraBoxCoordToMinimap(FVector BoxDotLocation, FName BoxDotMatName, FLinearColor& LastBoxDotCoord);
	void BoxDotBeyondBorderCheck(FVector LastBoxDotCoord, FName BoxDotMatName);
	void MoveBoxDotsToBoxCenter();
	void GetCameraUVPosition(FVector& CameraUVPosition);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void UVLineTraceByMinimap(FVector Start, FVector End);

	UFUNCTION(BlueprintImplementableEvent)
	void DrawFriendUnitLocationInfoToRenderTarget(FVector UnitLocation);

	UFUNCTION(BlueprintImplementableEvent)
	void DrawEnemyUnitLocationInfoToRenderTarget(FVector UnitLocation);

	UFUNCTION(BlueprintImplementableEvent)
	void DrawMinimapCameraBoxToRenderTarget();

public:
	AMinimap();

	virtual void Tick(float DeltaTime) override;

	float GetMinimapPlaneHalfLenght() const;
	UChildActorComponent* GetMinimapPlaneMoveStartDot() const;

	UFUNCTION(BlueprintCallable)
	UMaterialInstanceDynamic* GetMinimapPlaneMID() const;

	UFUNCTION(BlueprintCallable)
	UMaterialInstanceDynamic* GetMinimapCameraBoxMID() const;
};
