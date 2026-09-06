#include "WorldTreeManager.h"
#include "WorldTree.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

bool UWorldTreeManager::TryUnlockTree(AWorldTree* Tree)
{
    if (!IsValid(Tree))
    {
        return false;
    }

    const FName TreeID = Tree->GetWorldTreeID();
    if (TreeID.IsNone())
    {
        UE_LOG(LogTemp, Warning,
            TEXT("[WorldTree] WorldTreeID is None: %s"), *Tree->GetName());
        return false;
    }

    if (UnlockedTreesID.Contains(TreeID))
    {
        return false;
    }

    UnlockedTreesID.Add(TreeID);
    WorldTreeDataMap.Add(TreeID, Tree);

    OnWorldTreeUnlocked.Broadcast(TreeID);
    return true;
}

int32 UWorldTreeManager::PruneInvalidTrees()
{
    int32 RemovedCount = 0;

    for (int32 Index = UnlockedTreesID.Num() - 1; Index >= 0; --Index)
    {
        const FName TreeID = UnlockedTreesID[Index];
        const TWeakObjectPtr<AWorldTree>* Found = WorldTreeDataMap.Find(TreeID);

        if (Found && Found->IsValid())
        {
            continue;
        }

        UnlockedTreesID.RemoveAt(Index);
        WorldTreeDataMap.Remove(TreeID);
        ++RemovedCount;
    }

    return RemovedCount;
}

TArray<FName> UWorldTreeManager::GetUnlockedTrees()
{
    PruneInvalidTrees();
    return UnlockedTreesID;
}

bool UWorldTreeManager::IsTreeUnlockedByID(FName TreeID) const
{
    return UnlockedTreesID.Contains(TreeID);
}

bool UWorldTreeManager::IsTreeUnlockedByActor(const AWorldTree* Tree) const
{
    return IsValid(Tree) && UnlockedTreesID.Contains(Tree->GetWorldTreeID());
}

bool UWorldTreeManager::TeleportToWorldTree(FName TreeID)
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return false;
    }

    const double Now = World->GetTimeSeconds();
    if (LastTeleportTime >= 0.0 && (Now - LastTeleportTime) < TeleportCooldown)
    {
        return false;
    }

    if (!UnlockedTreesID.Contains(TreeID))
    {
        return false;
    }

    TWeakObjectPtr<AWorldTree>* Found = WorldTreeDataMap.Find(TreeID);
    if (!Found || !Found->IsValid())
    {
        PruneInvalidTrees();
        return false;
    }

    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!PlayerCharacter)
    {
        return false;
    }

    LastTeleportTime = Now;
    (*Found)->TeleportHereFromAnywhere(PlayerCharacter);
    return true;
}
