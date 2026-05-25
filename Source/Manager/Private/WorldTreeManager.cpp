// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldTreeManager.h"
#include "WorldTree.h"

void UWorldTreeManager::AddUnlockedTree(AWorldTree* Tree)
{
    if (IsValid(Tree) && !UnlockedTrees.Contains(Tree))
    {
        UnlockedTrees.Add(Tree);
    }
}

TArray<AWorldTree*> UWorldTreeManager::GetUnlockedTrees() const
{
    return UnlockedTrees;
}

bool UWorldTreeManager::IsTreeUnlocked(AWorldTree* Tree) const
{
    return UnlockedTrees.Contains(Tree);
}
