// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ElementalEnums.h"
#include "ElementalStone.generated.h"


class AElementalAltar;

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStoneActivated);


UCLASS()
class LUDENS7_TEAMPROJECT_API UElementalStone : public UObject
{
	GENERATED_BODY()
	
public:
	UElementalStone();
	void Initialize(EElementalType InType, float InMaxEnergy = 10.0f);

	void AddEnergy(float Amount);

	float GetProgress() const;
	inline float GetCurrentEnergy() const { return CurrentEnergy; };
	inline EElementalType GetElementalType() const { return ElementalType; }
	inline bool GetIsActivated() const { return bIsActivated; }

	FOnStoneActivated OnStoneActivated;

private:
	void Activate();

	EElementalType ElementalType;
	float MaxEnergy;
	float CurrentEnergy;
	bool bIsActivated;
};
