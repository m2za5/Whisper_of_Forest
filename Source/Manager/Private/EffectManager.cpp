// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectManager.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


void UEffectManager::Initialize(FSubsystemCollectionBase& Collection)
{
	World = GetWorld();
	PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0);
}

FName UEffectManager::PlayParticle(UParticleSystem* ParticleTemplate, FVector Location, FRotator Rotation)
{
	if (!World || !ParticleTemplate)
		return NAME_None;

	UParticleSystemComponent* ParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(World, ParticleTemplate, Location, Rotation);
	if (!ParticleComponent)
		return NAME_None;

	FName ParticleName = FName(*FString::Printf(TEXT("Particle_%d"), FMath::Rand()));
	ActiveParticleEffects.Add(ParticleName, ParticleComponent);

	return ParticleName;
}

FName UEffectManager::PlayParticleAtPlayer(UParticleSystem* ParticleTemplate, FRotator Rotation)
{
	return PlayParticle(ParticleTemplate, PlayerPawn->GetActorLocation(), Rotation);
}

void UEffectManager::StopParticle(FName ParticleName)
{
	if (!ActiveParticleEffects.Contains(ParticleName))
		return;

	UParticleSystemComponent* ParticleSystemComp = ActiveParticleEffects[ParticleName];
	if (ParticleSystemComp)
	{
		ParticleSystemComp->Deactivate();
	}
	ActiveParticleEffects.Remove(ParticleName);
}

void UEffectManager::StopParticleAfterTime(FName ParticleName, float Time)
{
	FTimerHandle TimerHandle;
	World->GetTimerManager().SetTimer(
		TimerHandle,
		FTimerDelegate::CreateLambda([this, ParticleName]()
			{
				StopParticle(ParticleName);
			}),
		Time,
		false
	);
}

FName UEffectManager::PlayeNiagara(UNiagaraSystem* NiagaraTemplate, FVector Location, FRotator Rotation)
{
	if (!World || !NiagaraTemplate)
		return NAME_None;

	UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		World,
		NiagaraTemplate,
		Location,
		Rotation
	);

	if (!NiagaraComp)
		return NAME_None;

	FName NiagaraName = FName(*FString::Printf(TEXT("Niagara_%d"), FMath::Rand()));
	ActiveNiagaraEffects.Add(NiagaraName, NiagaraComp);

	return NiagaraName;
}

FName UEffectManager::PlayeNiagaraAtPlayer(UNiagaraSystem* NiagaraTemplate, FVector Location, FRotator Rotation)
{
	return PlayeNiagara(NiagaraTemplate, PlayerPawn->GetActorLocation(), Rotation);
}

void UEffectManager::StopNiagara(FName NiagaraName)
{
	if (!ActiveNiagaraEffects.Contains(NiagaraName))
		return;

	UNiagaraComponent* NiagaraComp = ActiveNiagaraEffects[NiagaraName];
	if (NiagaraComp)
	{
		NiagaraComp->Deactivate();
	}
	ActiveParticleEffects.Remove(NiagaraName);
}

void UEffectManager::StopNiagaraAfterTime(FName NiagaraName, float Time)
{
	FTimerHandle TimerHandle;
	World->GetTimerManager().SetTimer(
		TimerHandle,
		FTimerDelegate::CreateLambda([this, NiagaraName]()
			{
				StopNiagara(NiagaraName);
			}),
		Time,
		false
	);
}

FName UEffectManager::PlayParticleAttached(UParticleSystem* ParticleTemplate, USceneComponent* AttachTo, FName SocketName, FVector Location, FRotator Rotation, EAttachLocation::Type LocationType)
{
	if (!ParticleTemplate || !AttachTo)
		return NAME_None;

	UParticleSystemComponent* ParticleComp = UGameplayStatics::SpawnEmitterAttached(
		ParticleTemplate,
		AttachTo,
		SocketName,
		Location,
		Rotation,
		LocationType
	);

	if (!ParticleComp)
		return NAME_None;

	FName ParticleName = FName(*FString::Printf(TEXT("ParticleAttach_%d"), FMath::Rand()));
	ActiveParticleEffects.Add(ParticleName, ParticleComp);

	return ParticleName;
}

FName UEffectManager::PlayNiagaraAttached(UNiagaraSystem* NiagaraTemplate, USceneComponent* AttachTo, FName SocketName, FVector Scale, FVector Location, FRotator Rotation, EAttachLocation::Type LocationType)
{
	if (!NiagaraTemplate || !AttachTo)
		return NAME_None;

	UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
		NiagaraTemplate,
		AttachTo,
		SocketName,
		Location,
		Rotation,
		LocationType,
		true
	);

	if (!NiagaraComp)
		return NAME_None;

	NiagaraComp->SetWorldScale3D(Scale);

	FName NiagaraName = FName(*FString::Printf(TEXT("NiagaraAttach_%d"), FMath::Rand()));
	ActiveNiagaraEffects.Add(NiagaraName, NiagaraComp);

	return NiagaraName;
}