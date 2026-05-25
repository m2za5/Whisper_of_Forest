// Fill out your copyright notice in the Description page of Project Settings.


#include "AltarPurifyComponent.h"
#include "CutsceneManager.h"
#include "LevelSequence/Public/LevelSequence.h"


UAltarPurifyComponent::UAltarPurifyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAltarPurifyComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UCutsceneManager* Manager = GetWorld()->GetSubsystem<UCutsceneManager>())
	{
		Manager->OnCutsceneFinished.AddDynamic(this, &UAltarPurifyComponent::OnCutsceneEnded);
	}	
}

void UAltarPurifyComponent::PlayPurifyCutscene()
{

	if (bCutsceneStarted || !CutsceneSequenceList.IsValidIndex(CurrentCutsceneIndex))
		return;

	if (UCutsceneManager* CutsceneManager = GetWorld()->GetSubsystem<UCutsceneManager>())
	{
		FName ID = GetCurrentCutsceneID();
		CutsceneManager->PlayCutscene(ID, CutsceneSequenceList[CurrentCutsceneIndex]);

		bCutsceneStarted = true;
	}
}

void UAltarPurifyComponent::OnCutsceneEnded(FName FinishedCutsceneID)
{
	CurrentCutsceneIndex++;

	if (CutsceneSequenceList.IsValidIndex(CurrentCutsceneIndex))
	{
		FName NewID = GetCurrentCutsceneID();
		UCutsceneManager* Manager = GetWorld()->GetSubsystem<UCutsceneManager>();
		if (Manager)
		{
			Manager->PlayCutscene(NewID, CutsceneSequenceList[CurrentCutsceneIndex]);
		}
	}
}


FName UAltarPurifyComponent::GetCurrentCutsceneID() const
{
	return FName(FString("Cutscene_") + FString::FromInt(CurrentCutsceneIndex));
}




