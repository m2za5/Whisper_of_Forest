#include "CutsceneManager.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequence/Public/LevelSequenceActor.h"

UCutsceneManager::UCutsceneManager()
{
}

void UCutsceneManager::Deinitialize()
{
    CutsceneStates.Empty();

    for (auto& Pair : ActiveSequencePlayer)
    {
        if (Pair.Value)
        {
            Pair.Value->Stop();
            Pair.Value->MarkAsGarbage();
        }
    }

    ActiveSequencePlayer.Empty();
    PlayerToCutsceneID.Empty();
}

void UCutsceneManager::PlayCutscene(FName CutsceneID, ULevelSequence* SequenceAsset)
{
    if (!SequenceAsset)
    {
        return;
    }

    if (ActiveSequencePlayer.Contains(CutsceneID))
    {
        return;
    }

    // 시퀀스 플레이어 생성
    FMovieSceneSequencePlaybackSettings PlaybackSettings;
    PlaybackSettings.bAutoPlay = false;

    ALevelSequenceActor* SequenceActor;

    ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
        GetWorld(), SequenceAsset, PlaybackSettings, SequenceActor);

    if (SequencePlayer)
    {
        ActiveSequencePlayer.Add(CutsceneID, SequencePlayer);
        PlayerToCutsceneID.Add(SequencePlayer, CutsceneID);
        CutsceneStates.FindOrAdd(CutsceneID) = static_cast<uint8>(ECutsceneState::Playing);

        // 시퀀스 종료 시 호출
        SequencePlayer->OnFinished.AddDynamic(this, &UCutsceneManager::OnCutsceneEnded);
        SequencePlayer->Play();

    }
}

void UCutsceneManager::OnCutsceneEnded()
{

    TArray<ULevelSequencePlayer*> CompletedPlayers;

    for (auto& Pair : PlayerToCutsceneID)
    {
        ULevelSequencePlayer* SequencePlayer = Pair.Key;
        FName CutsceneID = Pair.Value;

        if (SequencePlayer && !SequencePlayer->IsPlaying())
        {
            CutsceneStates[CutsceneID] |= static_cast<uint8>(ECutsceneState::Completed);
            OnCutsceneFinished.Broadcast(CutsceneID);

            CompletedPlayers.Add(SequencePlayer);
        }
    }

    // 종료된 플레이어 정리
    for (ULevelSequencePlayer* SequencePlayer : CompletedPlayers)
    {
        SequencePlayer->Stop();
        SequencePlayer->MarkAsGarbage();
        FName CutsceneID = PlayerToCutsceneID[SequencePlayer];

        ActiveSequencePlayer.Remove(CutsceneID);
        PlayerToCutsceneID.Remove(SequencePlayer);
    }
}

void UCutsceneManager::SkipCutscene()
{
    TArray<ULevelSequencePlayer*> PlayersToStop;

    for (auto& Pair : PlayerToCutsceneID)
    {
        ULevelSequencePlayer* SequencePlayer = Pair.Key;
        FName CutsceneID = Pair.Value;

        if (SequencePlayer && SequencePlayer->IsPlaying())
        {
            SequencePlayer->Stop();
            CutsceneStates[CutsceneID] |= static_cast<uint8>(ECutsceneState::Skipped);
            OnCutsceneFinished.Broadcast(CutsceneID);

            PlayersToStop.Add(SequencePlayer);
        }
    }

    // 정리
    for (ULevelSequencePlayer* SequencePlayer : PlayersToStop)
    {
        SequencePlayer->MarkAsGarbage();
        PlayerToCutsceneID.Remove(SequencePlayer);
    }
}

void UCutsceneManager::PauseCutscene()
{
    for (auto& Pair : ActiveSequencePlayer)
    {
        if (Pair.Value && Pair.Value->IsPlaying())
        {
            Pair.Value->Pause();
            CutsceneStates[Pair.Key] |= static_cast<uint8>(ECutsceneState::Paused); 
        }
    }
}

void UCutsceneManager::ResumeCutscene()
{
    for (auto& Pair : ActiveSequencePlayer)
    {
        if (Pair.Value && Pair.Value->IsPaused())
        {
            Pair.Value->Play();
            CutsceneStates[Pair.Key] |= static_cast<uint8>(ECutsceneState::Playing);
            CutsceneStates[Pair.Key] &= ~static_cast<uint8>(ECutsceneState::Paused);
        }
    }
}

bool UCutsceneManager::HasPlayedCutscene(FName CutsceneID) const
{
    return CutsceneStates.Contains(CutsceneID) && (CutsceneStates[CutsceneID] & static_cast<uint8>(ECutsceneState::Completed));
}

void UCutsceneManager::MarkCutsceneAsPlayed(FName CutsceneID)
{
    CutsceneStates.FindOrAdd(CutsceneID) |= static_cast<uint8>(ECutsceneState::Completed);
}
