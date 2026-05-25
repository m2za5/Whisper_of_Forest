#include "AltarCameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

UAltarCameraComponent::UAltarCameraComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UAltarCameraComponent::SetAltarCamera(AActor* Camera)
{
    AltarCamera = Camera;
}

AActor* UAltarCameraComponent::GetAltarCamera() const
{
    return AltarCamera;
}

void UAltarCameraComponent::SetZoomedCamera(AActor* Camera)
{
    ZoomedCamera = Camera;
}

AActor* UAltarCameraComponent::GetZoomedCamera() const
{
    return ZoomedCamera;
}

void UAltarCameraComponent::SetCameraMovementCurve(UCurveFloat* Curve)
{
    CameraMovementCurve = Curve;
}

UCurveFloat* UAltarCameraComponent::GetCameraMovementCurve() const
{
    return CameraMovementCurve;
}

void UAltarCameraComponent::InitializeCameraTimeline()
{
    if (!CameraMovementCurve)
    {
        return;
    }

    FOnTimelineFloat TimelineCallback;
    TimelineCallback.BindUFunction(this, FName("UpdateCameraPosition"));
    CameraTimeline.AddInterpFloat(CameraMovementCurve, TimelineCallback);

    CameraTimeline.SetLooping(false);
}

void UAltarCameraComponent::UpdateCameraPosition(float Value)
{
    FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, Value);
    FRotator NewRotation = FMath::Lerp(StartRotation, EndRotation, Value);

    if (AltarCamera)
    {
        AltarCamera->SetActorLocation(NewLocation);
        AltarCamera->SetActorRotation(NewRotation);
    }
}

void UAltarCameraComponent::ResetCameraLocation()
{
    if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        PlayerController->SetViewTargetWithBlend(PlayerController->GetPawn(), 0.0f);
    }
}
