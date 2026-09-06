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

	if (UCutsceneManager* Manager = GetCutsceneManager())
	{
		Manager->OnCutsceneFinished.RemoveDynamic(this, &UAltarPurifyComponent::OnCutsceneEnded);
		Manager->OnCutsceneFinished.AddDynamic(this, &UAltarPurifyComponent::OnCutsceneEnded);
	}
}

void UAltarPurifyComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UCutsceneManager* Manager = GetCutsceneManager())
	{
		Manager->OnCutsceneFinished.RemoveDynamic(this, &UAltarPurifyComponent::OnCutsceneEnded);
	}

	Super::EndPlay(EndPlayReason);
}

void UAltarPurifyComponent::PlayPurifyCutscene()
{
	if (bCutsceneStarted)
	{
		UE_LOG(LogTemp, Verbose, TEXT("[Purify] ?대? ?ъ깮 以? idx=%d"), CurrentCutsceneIndex);
		return;
	}

	UCutsceneManager* Manager = GetCutsceneManager();
	if (!Manager)
	{
		UE_LOG(LogTemp, Error, TEXT("[Purify] CutsceneManager瑜?李얠쓣 ???놁뒿?덈떎."));
		return;
	}

	if (Manager->IsCutscenePlaying())
	{
		UE_LOG(LogTemp, Warning, TEXT("[Purify] '%s' ?ъ깮 以묒씠???湲고빀?덈떎."),
			*Manager->GetActiveCutsceneID().ToString());
		return;
	}

	while (CutsceneSequenceList.IsValidIndex(CurrentCutsceneIndex))
	{
		ULevelSequence* Sequence = CutsceneSequenceList[CurrentCutsceneIndex];
		const FName ID = GetCurrentCutsceneID();

		if (Sequence && Manager->PlayCutscene(ID, Sequence))
		{
			ActivePurifyCutsceneID = ID;
			bCutsceneStarted = true;

			UE_LOG(LogTemp, Log, TEXT("[Purify] '%s' ?ъ깮 ?쒖옉. idx=%d"), *ID.ToString(), CurrentCutsceneIndex);
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("[Purify] '%s' ?ъ깮 遺덇? - ?ㅼ쓬 ?몃뜳?ㅻ줈. idx=%d"),
			*ID.ToString(), CurrentCutsceneIndex);

		++CurrentCutsceneIndex;
	}

	ActivePurifyCutsceneID = NAME_None;

	UE_LOG(LogTemp, Log, TEXT("[Purify] ?뺥솕 而룹떊 ?꾨? 醫낅즺. (珥?%d媛?"), CutsceneSequenceList.Num());
	OnPurifyCutscenesFinished.Broadcast();
}

void UAltarPurifyComponent::OnCutsceneEnded(FName FinishedCutsceneID)
{
	if (ActivePurifyCutsceneID.IsNone() || FinishedCutsceneID != ActivePurifyCutsceneID)
	{
		return;
	}

	bCutsceneStarted = false;
	ActivePurifyCutsceneID = NAME_None;
	++CurrentCutsceneIndex;

	PlayPurifyCutscene();
}

FName UAltarPurifyComponent::GetCurrentCutsceneID() const
{
	const AActor* Owner = GetOwner();
	const FString OwnerKey = Owner ? Owner->GetName() : TEXT("NoOwner");

	return FName(*FString::Printf(TEXT("Purify_%s_%d"), *OwnerKey, CurrentCutsceneIndex));
}

UCutsceneManager* UAltarPurifyComponent::GetCutsceneManager() const
{
	UWorld* World = GetWorld();
	return World ? World->GetSubsystem<UCutsceneManager>() : nullptr;
}
