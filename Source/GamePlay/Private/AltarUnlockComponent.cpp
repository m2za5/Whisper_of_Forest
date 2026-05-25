// Fill out your copyright notice in the Description page of Project Settings.


#include "AltarUnlockComponent.h"
#include "CutsceneManager.h"
#include "LevelSequence/Public/LevelSequence.h"


UAltarUnlockComponent::UAltarUnlockComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAltarUnlockComponent::PlayUnlockCutscene()
{
	UCutsceneManager* CutsceneManager = GetWorld()->GetSubsystem<UCutsceneManager>();
	if (!CutsceneManager || !UnlockSequence)
		return;

	
	CutsceneManager->OnCutsceneFinished.RemoveDynamic(this, &UAltarUnlockComponent::OnCutsceneFinished);
	CutsceneManager->OnCutsceneFinished.AddDynamic(this, &UAltarUnlockComponent::OnCutsceneFinished);
	CutsceneManager->PlayCutscene(CutsceneID, UnlockSequence);
}


void UAltarUnlockComponent::OnCutsceneFinished(FName FinishedID)
{
	if (FinishedID == CutsceneID)
	{
		OnUnlockCutsceneEnded.Broadcast();
	}
}


