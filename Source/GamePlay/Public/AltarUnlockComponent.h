// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AltarUnlockComponent.generated.h"


class ULevelSequence;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnlockCutsceneEnded);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LUDENS7_TEAMPROJECT_API UAltarUnlockComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAltarUnlockComponent();

	void PlayUnlockCutscene();

	FOnUnlockCutsceneEnded OnUnlockCutsceneEnded;

protected:
	UFUNCTION()
	void OnCutsceneFinished(FName FinishedID);


private:
	UPROPERTY(EditAnywhere, Category = "Cutscene")
	FName CutsceneID = "WaterAltarUnlockStone";

	UPROPERTY(EditAnywhere, Category = "Cutscene")
	ULevelSequence* UnlockSequence;
};
