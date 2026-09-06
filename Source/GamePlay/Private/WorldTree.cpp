#include "WorldTree.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "WorldTreeManager.h"

AWorldTree::AWorldTree()
{
    PrimaryActorTick.bCanEverTick = false;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;
    TriggerBox->SetBoxExtent(FVector(100.f));
    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    TriggerBox->SetCollisionResponseToChannel(InteractionTraceChannel, ECR_Block);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual"));
    Mesh->SetupAttachment(RootComponent);
}


void AWorldTree::TeleportHereFromAnywhere(ACharacter* Player)
{
    if (!Player) return;

    UE_LOG(LogTemp, Warning, TEXT("WorldTree : Yes"));

    if (!bUnlocked) return;

    const FVector TargetLocation = GetActorLocation() + FVector(0.f, 0.f, 50.f);
    const FRotator TargetRotation = GetActorRotation();

    Player->TeleportTo(TargetLocation, TargetRotation, false, true);
}

void AWorldTree::Unlock()
{
    if (bUnlocked) return;

    UWorldTreeManager* WTM = GetWorld()->GetSubsystem<UWorldTreeManager>();
    if (!WTM || !WTM->TryUnlockTree(this))
    {
        return;
    }

    bUnlocked = true;

    if (TriggerBox)
    {
        TriggerBox->SetCollisionResponseToChannel(InteractionTraceChannel, ECR_Ignore);
    }
}

FString AWorldTree::Interact_Implementation(AActor* Interactor)
{
    Unlock();
    return TEXT("");
}

void AWorldTree::OnFocus_Implementation(AActor* Looker)
{
}

void AWorldTree::OnUnfocus_Implementation(AActor* Looker)
{
}

void AWorldTree::GetInteractionTriggers_Implementation(TArray<UPrimitiveComponent*>& OutTriggers)
{
    if (TriggerBox)
    {
        OutTriggers.Add(TriggerBox);
    }
}

FText AWorldTree::GetInteractText_Implementation()
{
    if (!bUnlocked)
    {
        return FText::FromString(TEXT("세계수 등록"));
    }
    return FText::GetEmpty();
}
