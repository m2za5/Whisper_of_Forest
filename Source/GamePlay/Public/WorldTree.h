// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldTree.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class ACharacter;

UCLASS()
class LUDENS7_TEAMPROJECT_API AWorldTree : public AActor
{
	GENERATED_BODY()
	
public:	
	AWorldTree();

protected:

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


    void ReEnableOverlap();


public:
    UFUNCTION(BlueprintCallable, Category = "WorldTree")
    void TeleportHereFromAnywhere(ACharacter* Player);

    UFUNCTION(BlueprintCallable, Category = "WorldTree")
    void Unlock() { bUnlocked = true; }

    UFUNCTION(BlueprintPure, Category = "WorldTree")
    bool IsUnlocked() const { return bUnlocked; }

private:
    void SetOverlapEnabled(bool bEnabled);

    UPROPERTY(VisibleAnywhere)
    UBoxComponent* TriggerBox;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Mesh;


    UPROPERTY(EditAnywhere, Category = "Teleport|Safety")
    float OverlapCooldown = 0.25f;

    UPROPERTY(EditAnywhere, Category = "WorldTree")
    bool bUnlocked = false;

    FTimerHandle ReEnableOverlapHandle;
};
