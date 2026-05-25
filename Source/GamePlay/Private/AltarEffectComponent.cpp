#include "AltarEffectComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

UAltarEffectComponent::UAltarEffectComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}
