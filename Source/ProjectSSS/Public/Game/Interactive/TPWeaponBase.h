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

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	FTransform WeaponEquipTransform;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	FTransform WeaponBackTransform_Left;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	FTransform WeaponBackTransform_Right;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	FTransform HandPosAndRot_Left;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	FTransform HandPosAndRot_Right;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	bool bEnableHandPosRot_Left;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	bool bEnableHandPosRot_Right;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual  void OnConstruction(const FTransform& Transform) override;

	virtual void Destroyed() override;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Weapon",meta=(AllowPrivateAccess="true"))
	bool bWeaponActive;
	
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Weapon",meta=(AllowPrivateAccess="true"))
	class USkeletalMeshComponent* Weapon;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Billboard",meta=(AllowPrivateAccess="true"))
	class UBillboardComponent* Billboard;

	class UTPWorldSubsystem* WorldSubsystem;
	
};
