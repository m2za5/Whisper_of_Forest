#include "AltarTimelineComponent.h"
#include "Components/TimelineComponent.h"
#include "Engine/PostProcessVolume.h"
#include "Kismet/KismetMathLibrary.h"

UAltarTimelineComponent::UAltarTimelineComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}
