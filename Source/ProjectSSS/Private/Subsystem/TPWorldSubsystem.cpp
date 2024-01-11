// Fill out your copyright notice in the Description page of Project Settings.

#include "Subsystem/TPWorldSubsystem.h"
#include "Game/TPWorldManager.h"
#include "Kismet/GameplayStatics.h"

void UTPWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UTPWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GetWorldManager();
}

void UTPWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
}

void UTPWorldSubsystem::BeginDestroy()
{
	Super::BeginDestroy();
	
}

TStatId UTPWorldSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UTPWorldSubsystem, STATGROUP_Tickables);
}

ATPWorldManager* UTPWorldSubsystem::GetWorldManager()
{
	if(!IsValid(WorldManager))
	{
		TArray<AActor*> worldManagerActor;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(),ATPWorldManager::StaticClass(),worldManagerActor);
		if(worldManagerActor.IsEmpty())
		{
			WorldManager = GetWorld()->SpawnActor<ATPWorldManager>(
	FVector(0,0,0),FRotator(0,0,0));
			WorldManager->SetActorLabel(TEXT("World Manager"));
		}
		else
		{
			WorldManager = Cast<ATPWorldManager>(worldManagerActor[0]);
		}
	}
	return WorldManager;
}