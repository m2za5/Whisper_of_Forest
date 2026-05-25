#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Delegates/DelegateCombinations.h"
#include "LevelSequence/Public/LevelSequence.h"
#include "LevelSequence/Public/LevelSequencePlayer.h"
#include "CutsceneManager.generated.h"

// 컷신 종료 시 알림 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCutsceneFinished, FName, CutsceneID);

UENUM(BlueprintType)
enum class ECutsceneState : uint8
{
    None = 0,
    Playing = 1 << 0,
    Paused = 1 << 1,
    Skipped = 1 << 2,
    Completed = 1 << 3
};

UCLASS(BlueprintType)
class LUDENS7_TEAMPROJECT_API UCutsceneManager : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UCutsceneManager();
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "Cutscene")
    void PlayCutscene(FName CutsceneID, ULevelSequence* SequenceAsset);

    UFUNCTION(BlueprintCallable, Category = "Cutscene")
    void SkipCutscene();

    UFUNCTION(BlueprintCallable, Category = "Cutscene")
    void PauseCutscene();

    UFUNCTION(BlueprintCallable, Category = "Cutscene")
    void ResumeCutscene();

    UFUNCTION(BlueprintCallable, Category = "Cutscene")
    bool HasPlayedCutscene(FName CutsceneID) const;

    UFUNCTION(BlueprintCallable, Category = "Cutscene")
    void MarkCutsceneAsPlayed(FName CutsceneID);

    UPROPERTY(BlueprintAssignable, Category = "Cutscene")
    FOnCutsceneFinished OnCutsceneFinished;

private:
    // 컷신 상태 관리
    TMap<FName, uint8> CutsceneStates;

    // CutsceneID와 ULevelSequencePlayer 매핑
    TMap<FName, ULevelSequencePlayer*> ActiveSequencePlayer;

    // ULevelSequencePlayer와 CutsceneID 매핑
    TMap<ULevelSequencePlayer*, FName> PlayerToCutsceneID;

    // 시퀀스 종료 시 호출될 함수
    UFUNCTION()
    void OnCutsceneEnded();
};
