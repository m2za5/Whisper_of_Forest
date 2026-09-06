// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EffectManager.generated.h"


class APawn;
class UParticleSystemComponent;
class UNiagaraComponent;

// 파티클/시스템 내부적으로 오브젝트 풀링 처리 O
UCLASS()
class LUDENS7_TEAMPROJECT_API UEffectManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION()
	FName PlayParticle(UParticleSystem* ParticleTemplate, FVector Location, FRotator Rotation = FRotator::ZeroRotator);

	UFUNCTION()
	FName PlayParticleAtPlayer(UParticleSystem* ParticleTemplate, FRotator Rotation = FRotator::ZeroRotator);

	UFUNCTION()
	void StopParticle(FName ParticleName);

	UFUNCTION()
	void StopParticleAfterTime(FName ParticleName, float Time);

	UFUNCTION()
	FName PlayeNiagara(UNiagaraSystem* NiagaraTemplate, FVector Location, FRotator Rotation = FRotator::ZeroRotator);

	UFUNCTION()
	FName PlayeNiagaraAtPlayer(UNiagaraSystem* NiagaraTemplate, FVector Location, FRotator Rotation = FRotator::ZeroRotator);

	UFUNCTION()
	void StopNiagara(FName NiagaraName);

	UFUNCTION()
	void StopNiagaraAfterTime(FName NiagaraName, float Time);

	UFUNCTION()
	FName PlayParticleAttached(UParticleSystem* ParticleTemplate, USceneComponent* AttachTo, FName SocketName, FVector Location = FVector::ZeroVector, FRotator Rotation = FRotator::ZeroRotator, EAttachLocation::Type LocationType = EAttachLocation::KeepRelativeOffset);

	UFUNCTION()
	FName PlayNiagaraAttached(UNiagaraSystem* NiagaraTemplate, USceneComponent* AttachTo, FName SocketName, FVector Scale = FVector(1.0f, 1.0f, 1.0f), FVector Location = FVector::ZeroVector, FRotator Rotation = FRotator::ZeroRotator, EAttachLocation::Type LocationType = EAttachLocation::KeepRelativeOffset);

private:
	static TWeakObjectPtr<UEffectManager> Instance;

	TMap<FName, UParticleSystemComponent*> ActiveParticleEffects;
	TMap<FName, UNiagaraComponent*> ActiveNiagaraEffects;

	UWorld* World;
	APawn* PlayerPawn;
};
