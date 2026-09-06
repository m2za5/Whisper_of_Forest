#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Delegates/DelegateCombinations.h"
#include "LevelSequence/Public/LevelSequence.h"
#include "LevelSequence/Public/LevelSequencePlayer.h"
#include "CutsceneManager.generated.h"

// 컷신 종료 시 알림 델리게이트
class ULevelSequence;
class ULevelSequencePlayer;
class ALevelSequenceActor;
class UCutsceneFinishedProxy;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCutsceneStarted, FName, CutsceneID);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCutsceneFinished, FName, CutsceneID);

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ECutsceneState : uint8
{
	None = 0,
	Playing = 1 << 0,
	Paused = 1 << 1,
	Skipped = 1 << 2,
	Completed = 1 << 3
};
ENUM_CLASS_FLAGS(ECutsceneState);
USTRUCT()
struct FActiveCutscene
{
    GENERATED_BODY()

    UPROPERTY()
    FName ID = NAME_None;

    UPROPERTY()
    TObjectPtr<ULevelSequencePlayer> Player = nullptr;

    UPROPERTY()
    TObjectPtr<ALevelSequenceActor> Actor = nullptr;

    UPROPERTY()
    TObjectPtr<UCutsceneFinishedProxy> Proxy = nullptr;

    bool IsValid() const { return !ID.IsNone() && Player != nullptr; }

    void Reset() { *this = FActiveCutscene(); }
};

UCLASS(BlueprintType)
class LUDENS7_TEAMPROJECT_API UCutsceneManager : public UWorldSubsystem
{
    GENERATED_BODY()

public:
	UCutsceneManager();

	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "Cutscene")
	bool PlayCutscene(FName CutsceneID, ULevelSequence* SequenceAsset);

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
	UFUNCTION(BlueprintPure, Category = "Cutscene")
	bool IsCutscenePlaying() const { return ActiveCutscene.IsValid(); }

	UFUNCTION(BlueprintPure, Category = "Cutscene")
	FName GetActiveCutsceneID() const { return ActiveCutscene.ID; }

	UPROPERTY(BlueprintAssignable, Category = "Cutscene")
	FOnCutsceneStarted OnCutsceneStarted;

	UPROPERTY(BlueprintAssignable, Category = "Cutscene")
	FOnCutsceneFinished OnCutsceneFinished;
	void NotifyCutsceneFinished(FName CutsceneID);

private:
	void TearDownActiveCutscene();
	void SetPlayerHUDVisible(bool bVisible) const;
	TMap<FName, ECutsceneState> CutsceneStates;

	UPROPERTY()
	FActiveCutscene ActiveCutscene;
};
