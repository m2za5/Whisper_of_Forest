#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WorldTreeManager.generated.h"

class AWorldTree;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWorldTreeUnlocked, FName, WorldTreeID);

UCLASS()
class LUDENS7_TEAMPROJECT_API UWorldTreeManager : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category = "WorldTree")
    FOnWorldTreeUnlocked OnWorldTreeUnlocked;

public:
    UFUNCTION(BlueprintCallable, Category = "WorldTree")
    bool TryUnlockTree(AWorldTree* Tree);

    UFUNCTION(BlueprintCallable, Category = "WorldTree")
    TArray<FName> GetUnlockedTrees();

    UFUNCTION(BlueprintPure, Category = "WorldTree")
    bool IsTreeUnlockedByID(FName TreeID) const;

    UFUNCTION(BlueprintPure, Category = "WorldTree")
    bool IsTreeUnlockedByActor(const AWorldTree* Tree) const;

    UFUNCTION(BlueprintCallable, Category = "WorldTree")
    bool TeleportToWorldTree(FName TreeID);

private:
    int32 PruneInvalidTrees();

    UPROPERTY(EditDefaultsOnly, Category = "WorldTree")
    float TeleportCooldown = 0.5f;

    UPROPERTY()
    TArray<FName> UnlockedTreesID;

    UPROPERTY()
    TMap<FName, TWeakObjectPtr<AWorldTree>> WorldTreeDataMap;

    double LastTeleportTime = -1.0;
};
