// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "WorldTree.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class ACharacter;

UCLASS()
class LUDENS7_TEAMPROJECT_API AWorldTree : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	AWorldTree();

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WorldTree")
    FName WorldTreeID;

public:
    // Interface
    virtual FString Interact_Implementation(AActor* Interactor) override;
    virtual void OnFocus_Implementation(AActor* Looker) override;
    virtual void OnUnfocus_Implementation(AActor* Looker) override;
    virtual void GetInteractionTriggers_Implementation(TArray<UPrimitiveComponent*>& OutTriggers) override;
    virtual FText GetInteractText_Implementation() override;

    UFUNCTION(BlueprintCallable, Category = "WorldTree")
    void TeleportHereFromAnywhere(ACharacter* Player);

    UFUNCTION(BlueprintCallable, Category = "WorldTree")
    void Unlock();

    UFUNCTION(BlueprintPure, Category = "WorldTree")
    bool IsUnlocked() const { return bUnlocked; }

    UFUNCTION(BlueprintPure, Category = "WorldTree")
    FName GetWorldTreeID() const { return WorldTreeID; }

private:
    UPROPERTY(VisibleAnywhere)
    UBoxComponent* TriggerBox;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere, Category = "WorldTree")
    bool bUnlocked = false;
};
