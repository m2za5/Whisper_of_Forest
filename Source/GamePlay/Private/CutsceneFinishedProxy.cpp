// Fill out your copyright notice in the Description page of Project Settings.
#include "CutsceneFinishedProxy.h"
#include "CutsceneManager.h"


void UCutsceneFinishedProxy::Init(UCutsceneManager* InManager, FName InCutsceneID)
{
	Manager = InManager;
	CutsceneID = InCutsceneID;
}

void UCutsceneFinishedProxy::OnSequenceFinished()
{
	if (UCutsceneManager* CutsceneManager = Manager.Get())
	{
		CutsceneManager->NotifyCutsceneFinished(CutsceneID);
	}
}
