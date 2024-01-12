// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/Interactive/TPInteractiveActor.h"
#include "TPWeaponBase.generated.h"

UCLASS()
class PROJECTSSS_API ATPWeaponBase : public ATPInteractiveActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPWeaponBase();

	inline class USkeletalMeshComponent*  GetWeaponComp()const{return Weapon;}

	UFUNCTION(BlueprintCallable,Category="Interactive")
	virtual FVector GetInteractiveLocation() override;

	UFUNCTION(BlueprintCallable)
	bool IsWeaponActive()const
	{
		return bWeaponActive;
	}

	UFUNCTION(BlueprintCallable)
	virtual void Equip(class ATPCharacterBase* weaponOwner);

	UFUNCTION(BlueprintCallable)
	virtual void UnEquip(ATPWeaponBase* newWeapon);

	UFUNCTION(BlueprintCallable)
	virtual void PickUp(class ATPCharacterBase* weaponOwner);

	UFUNCTION(BlueprintCallable)
	virtual void Drop(FVector dropLocation);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	FTransform WeaponEquipTransform;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	FTransform WeaponBackTransform;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual  void OnConstruction(const FTransform& Transform) override;

	virtual void Destroyed() override;
	
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Weapon",meta=(AllowPrivateAccess="true"))
	class USkeletalMeshComponent* Weapon;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Billboard",meta=(AllowPrivateAccess="true"))
	class UBillboardComponent* Billboard;

	class UTPWorldSubsystem* WorldSubsystem;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Weapon",meta=(AllowPrivateAccess="true"))
	bool bWeaponActive;
};
