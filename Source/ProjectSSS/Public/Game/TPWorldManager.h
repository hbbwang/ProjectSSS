// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPWorldManager.generated.h"

UCLASS()
class PROJECTSSS_API ATPWorldManager : public AActor
{
	GENERATED_BODY()
	friend  class ATPWeaponBase;	
public:	
	// Sets default values for this actor's properties
	ATPWorldManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	virtual bool ShouldTickIfViewportsOnly() const override{return true;}
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	inline const TArray<ATPWeaponBase*> GetWeapons()const
	{
		return Weapons;
	}
	
private:
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="WorldManager",meta=(AllowPrivateAccess="true"))
	TArray<ATPWeaponBase*> Weapons; 
	
};
