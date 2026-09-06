// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementalStone.h"
#include "ElementalAltar.h"

namespace
{
	constexpr float DefaultStoneMaxEnergy = 10.0f;
}

UElementalStone::UElementalStone()
{
	MaxEnergy = 10.0f;
	CurrentEnergy = 0.0f;
	bIsActivated = false;
}

void UElementalStone::Initialize(EElementalType InType, float InMaxEnergy)
{
	ElementalType = InType;
	if (InMaxEnergy <= KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Error,
			TEXT("[ElementalStone] Invalid MaxEnergy(%f). Fallback to %f."),
			InMaxEnergy, DefaultStoneMaxEnergy);

		MaxEnergy = DefaultStoneMaxEnergy;
	}
	else
	{
		MaxEnergy = InMaxEnergy;
	}

	CurrentEnergy = 0.0f;
	bIsActivated = false;
}

void UElementalStone::AddEnergy(float Amount)
{
	if (bIsActivated)
		return;

	CurrentEnergy = FMath::Clamp(CurrentEnergy + Amount, 0.0f, MaxEnergy);
	UE_LOG(LogTemp, Warning, TEXT("CurrentEnergy: %f, MaxEnergy: %f"), CurrentEnergy, MaxEnergy);

	if (CurrentEnergy >= MaxEnergy) 
	{
		Activate();
	}
}

float UElementalStone::GetProgress() const
{
	return MaxEnergy > KINDA_SMALL_NUMBER ? (CurrentEnergy / MaxEnergy) : 0.0f;
}

void UElementalStone::Activate()
{
	if (bIsActivated)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Activate"));
	bIsActivated = true;
	OnStoneActivated.Broadcast();
}
