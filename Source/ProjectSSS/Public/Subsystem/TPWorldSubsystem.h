// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TickableEditorObject.h"
#include "Subsystems/WorldSubsystem.h"
#include "TPWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSSS_API UTPWorldSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
private:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual bool IsTickableInEditor() const override{return true;}

	virtual void Tick(float DeltaTime) override;
	
	void OnWorldBeginPlay(UWorld& InWorld) override;

	void BeginDestroy() override;
	
	virtual TStatId GetStatId() const override;

	class ATPWorldManager* WorldManager;
	
public:

	class ATPWorldManager* GetWorldManager();
	
	
	
};
