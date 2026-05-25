// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AltarPurifyComponent.generated.h"


class UCutsceneManager;
class ULevelSequence;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LUDENS7_TEAMPROJECT_API UAltarPurifyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAltarPurifyComponent();

	void PlayPurifyCutscene();

	UFUNCTION()
	void OnCutsceneEnded(FName FinishedCutsceneID);


protected:
	virtual void BeginPlay() override;


private:
	FName GetCurrentCutsceneID() const;

	UPROPERTY(EditAnywhere, Category = "Cutscene")
	TArray<ULevelSequence*> CutsceneSequenceList;

	bool bCutsceneStarted = false;
	int32 CurrentCutsceneIndex = 0;
};
