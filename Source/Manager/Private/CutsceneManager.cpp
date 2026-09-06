#include "CutsceneManager.h"
#include "CutsceneFinishedProxy.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"

#include "HUDWidget.h"
#include "PlayerCharacter.h"
#include "UIComponent.h"

UCutsceneManager::UCutsceneManager()
{
}

void UCutsceneManager::Deinitialize()
{
	TearDownActiveCutscene();
	CutsceneStates.Empty();

	SetPlayerHUDVisible(true);

	Super::Deinitialize();
}

bool UCutsceneManager::PlayCutscene(FName CutsceneID, ULevelSequence* SequenceAsset)
{
	if (CutsceneID.IsNone() || !SequenceAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayCutscene 거부: ID 또는 시퀀스 에셋이 유효하지 않음."));
		return false;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	ECutsceneState& State = CutsceneStates.FindOrAdd(CutsceneID);

	if (EnumHasAnyFlags(State, ECutsceneState::Completed))
	{
		UE_LOG(LogTemp, Verbose, TEXT("'%s'은(는) 이미 완료된 컷신이므로 재생하지 않음."), *CutsceneID.ToString());
		return false;
	}

	if (ActiveCutscene.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("'%s' 재생 거부: 현재 '%s' 재생 중."),
			*CutsceneID.ToString(), *ActiveCutscene.ID.ToString());
		return false;
	}

	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	PlaybackSettings.bAutoPlay = false;

	ALevelSequenceActor* SequenceActor = nullptr;
	ULevelSequencePlayer* SequencePlayer =
		ULevelSequencePlayer::CreateLevelSequencePlayer(World, SequenceAsset, PlaybackSettings, SequenceActor);

	if (!SequencePlayer)
	{
		if (SequenceActor)
		{
			SequenceActor->Destroy();
		}

		UE_LOG(LogTemp, Error, TEXT("'%s' LevelSequencePlayer 생성 실패."), *CutsceneID.ToString());
		return false;
	}

	UCutsceneFinishedProxy* Proxy = NewObject<UCutsceneFinishedProxy>(this);
	Proxy->Init(this, CutsceneID);

	ActiveCutscene.ID = CutsceneID;
	ActiveCutscene.Player = SequencePlayer;
	ActiveCutscene.Actor = SequenceActor;
	ActiveCutscene.Proxy = Proxy;

	SequencePlayer->OnFinished.AddDynamic(Proxy, &UCutsceneFinishedProxy::OnSequenceFinished);

	State |= ECutsceneState::Playing;
	State &= ~ECutsceneState::Paused;

	SetPlayerHUDVisible(false);
	OnCutsceneStarted.Broadcast(CutsceneID);

	UE_LOG(LogTemp, Log, TEXT("'%s' 재생 시작."), *CutsceneID.ToString());
	SequencePlayer->Play();

	return true;
}

void UCutsceneManager::SkipCutscene()
{
	if (!ActiveCutscene.IsValid())
	{
		return;
	}

	ULevelSequencePlayer* Player = ActiveCutscene.Player;
	if (!Player->IsPlaying() && !Player->IsPaused())
	{
		return;
	}

	const FName SkippedID = ActiveCutscene.ID;
	CutsceneStates.FindOrAdd(SkippedID) |= ECutsceneState::Skipped;

	UE_LOG(LogTemp, Log, TEXT("'%s' 스킵."), *SkippedID.ToString());
	NotifyCutsceneFinished(SkippedID);
}

void UCutsceneManager::PauseCutscene()
{
	if (!ActiveCutscene.IsValid())
	{
		return;
	}

	ULevelSequencePlayer* Player = ActiveCutscene.Player;
	if (!Player->IsPlaying())
	{
		return;
	}

	Player->Pause();
	CutsceneStates.FindOrAdd(ActiveCutscene.ID) |= ECutsceneState::Paused;
}

void UCutsceneManager::ResumeCutscene()
{
	if (!ActiveCutscene.IsValid())
	{
		return;
	}

	ULevelSequencePlayer* Player = ActiveCutscene.Player;
	if (!Player->IsPaused())
	{
		return;
	}

	Player->Play();

	ECutsceneState& State = CutsceneStates.FindOrAdd(ActiveCutscene.ID);
	State |= ECutsceneState::Playing;
	State &= ~ECutsceneState::Paused;
}

bool UCutsceneManager::HasPlayedCutscene(FName CutsceneID) const
{
	const ECutsceneState* State = CutsceneStates.Find(CutsceneID);
	return State != nullptr && EnumHasAnyFlags(*State, ECutsceneState::Completed);
}

void UCutsceneManager::MarkCutsceneAsPlayed(FName CutsceneID)
{
	if (CutsceneID.IsNone())
	{
		return;
	}

	CutsceneStates.FindOrAdd(CutsceneID) |= ECutsceneState::Completed;
}

void UCutsceneManager::NotifyCutsceneFinished(FName CutsceneID)
{
	{
		ECutsceneState& State = CutsceneStates.FindOrAdd(CutsceneID);
		if (EnumHasAnyFlags(State, ECutsceneState::Completed))
		{
			return;
		}

		State &= ~(ECutsceneState::Playing | ECutsceneState::Paused);
		State |= ECutsceneState::Completed;
	}

	if (ActiveCutscene.ID == CutsceneID)
	{
		TearDownActiveCutscene();
	}

	UE_LOG(LogTemp, Log, TEXT("'%s' 종료."), *CutsceneID.ToString());
	OnCutsceneFinished.Broadcast(CutsceneID);
	if (!ActiveCutscene.IsValid())
	{
		SetPlayerHUDVisible(true);
	}
}

void UCutsceneManager::TearDownActiveCutscene()
{
	if (ULevelSequencePlayer* Player = ActiveCutscene.Player.Get())
	{
		if (IsValid(Player))
		{
			if (UCutsceneFinishedProxy* Proxy = ActiveCutscene.Proxy.Get())
			{
				Player->OnFinished.RemoveDynamic(Proxy, &UCutsceneFinishedProxy::OnSequenceFinished);
			}

			Player->Stop();
		}
	}

	if (ALevelSequenceActor* Actor = ActiveCutscene.Actor.Get())
	{
		if (IsValid(Actor))
		{
			Actor->Destroy();
		}
	}

	ActiveCutscene.Reset();
}

void UCutsceneManager::SetPlayerHUDVisible(bool bVisible) const
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
	if (!PlayerController)
	{
		return;
	}

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetPawn());
	if (!PlayerCharacter)
	{
		return;
	}

	UUIComponent* UIComponent = PlayerCharacter->GetUIComponent();
	if (!UIComponent || !UIComponent->MyHUD)
	{
		return;
	}

	UIComponent->MyHUD->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}
