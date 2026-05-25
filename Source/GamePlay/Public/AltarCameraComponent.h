#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/TimelineComponent.h"
#include "AltarCameraComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LUDENS7_TEAMPROJECT_API UAltarCameraComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAltarCameraComponent();

    void SetAltarCamera(AActor* Camera);
    AActor* GetAltarCamera() const;

    void SetZoomedCamera(AActor* Camera);
    AActor* GetZoomedCamera() const;

    void SetCameraMovementCurve(UCurveFloat* Curve);
    UCurveFloat* GetCameraMovementCurve() const;

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void InitializeCameraTimeline();

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void ResetCameraLocation();

private:
    FTimeline CameraTimeline;

    UPROPERTY()
    AActor* AltarCamera;

    UPROPERTY()
    AActor* ZoomedCamera;

    UPROPERTY()
    UCurveFloat* CameraMovementCurve;

    FVector StartLocation;
    FRotator StartRotation;

    FVector EndLocation;
    FRotator EndRotation;

    void UpdateCameraPosition(float Value);
};
