// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CutsceneFinishedProxy.generated.h"
class UCutsceneManager;

UCLASS()
class LUDENS7_TEAMPROJECT_API UCutsceneFinishedProxy : public UObject
{
	GENERATED_BODY()

public:
	void Init(UCutsceneManager* InManager, FName InCutsceneID);

	UFUNCTION()
	void OnSequenceFinished();

private:
	UPROPERTY()
	TObjectPtr<UCutsceneManager> Manager = nullptr;

	UPROPERTY()
	FName CutsceneID = NAME_None;
};
