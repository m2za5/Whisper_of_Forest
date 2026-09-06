// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AltarPurifyComponent.generated.h"


class UCutsceneManager;
class ULevelSequence;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPurifyCutscenesFinished);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LUDENS7_TEAMPROJECT_API UAltarPurifyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAltarPurifyComponent();

	UFUNCTION(BlueprintCallable, Category = "Cutscene")
	void PlayPurifyCutscene();

	UFUNCTION()
	void OnCutsceneEnded(FName FinishedCutsceneID);

	UPROPERTY(BlueprintAssignable, Category = "Cutscene")
	FOnPurifyCutscenesFinished OnPurifyCutscenesFinished;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	FName GetCurrentCutsceneID() const;
	UCutsceneManager* GetCutsceneManager() const;

	UPROPERTY(EditAnywhere, Category = "Cutscene")
	TArray<TObjectPtr<ULevelSequence>> CutsceneSequenceList;

	bool bCutsceneStarted = false;
	int32 CurrentCutsceneIndex = 0;

	FName ActivePurifyCutsceneID = NAME_None;
};
