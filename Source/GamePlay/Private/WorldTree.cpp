#include "WorldTree.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "PlayerCharacter.h"

AWorldTree::AWorldTree()
{
    PrimaryActorTick.bCanEverTick = false;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;
    TriggerBox->SetBoxExtent(FVector(100.f));
    TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AWorldTree::OnOverlapBegin);
    TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AWorldTree::OnOverlapEnd);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual"));
    Mesh->SetupAttachment(RootComponent);
}


void AWorldTree::SetOverlapEnabled(bool bEnabled)
{
    if (!TriggerBox) return;
    TriggerBox->SetGenerateOverlapEvents(bEnabled);
}

void AWorldTree::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (APlayerCharacter* PC = Cast<APlayerCharacter>(OtherActor))
    {
        PC->FocusedTree = this;
    }

}

void AWorldTree::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (APlayerCharacter* PC = Cast<APlayerCharacter>(OtherActor))
    {
        if (PC->FocusedTree == this)
        {
            PC->FocusedTree = nullptr;
        }
    }
}

void AWorldTree::ReEnableOverlap()
{
    SetOverlapEnabled(true);
}

void AWorldTree::TeleportHereFromAnywhere(ACharacter* Player)
{
    if (!Player) return;
    if (!bUnlocked) return;

    const FVector TargetLocation = GetActorLocation() + FVector(0.f, 0.f, 50.f);
    const FRotator TargetRotation = GetActorRotation();

    SetOverlapEnabled(false);

    Player->TeleportTo(TargetLocation, TargetRotation, false, true);

    GetWorldTimerManager().SetTimer(
        ReEnableOverlapHandle, this, &AWorldTree::ReEnableOverlap, OverlapCooldown, false);
}
