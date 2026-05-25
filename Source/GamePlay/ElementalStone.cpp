// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementalStone.h"
#include "ElementalAltar.h"

UElementalStone::UElementalStone()
{
	MaxEnergy = 10.0f;
	CurrentEnergy = 0.0f;
	bIsActivated = false;
}

void UElementalStone::Initialize(EElementalType InType, float InMaxEnergy)
{
	ElementalType = InType;
	MaxEnergy = InMaxEnergy;
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
	return CurrentEnergy / MaxEnergy;
}

void UElementalStone::Activate()
{
	if (bIsActivated)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Activate"));
	bIsActivated = true;
	OnStoneActivated.Broadcast();
}
