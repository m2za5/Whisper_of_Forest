// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WorldTreeManager.generated.h"
class AWorldTree;

/**
 * 
 */
UCLASS()
class LUDENS7_TEAMPROJECT_API UWorldTreeManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "WorldTree")
    void AddUnlockedTree(AWorldTree* Tree);

    UFUNCTION(BlueprintPure, Category = "WorldTree")
    TArray<AWorldTree*> GetUnlockedTrees() const;

    UFUNCTION(BlueprintPure, Category = "WorldTree")
    bool IsTreeUnlocked(AWorldTree* Tree) const;

private:
    UPROPERTY()
    TArray<AWorldTree*> UnlockedTrees;

};
