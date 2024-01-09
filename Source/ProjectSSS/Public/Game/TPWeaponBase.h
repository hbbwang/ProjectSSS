// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPWeaponBase.generated.h"

UCLASS()
class PROJECTSSS_API ATPWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPWeaponBase();

	inline class USkeletalMeshComponent*  GetWeaponComp()const{return Weapon;}

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Weapon")
	bool bEquip;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Weapon")
	TWeakObjectPtr<AActor> WeaponOwner;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual  void OnConstruction(const FTransform& Transform) override;
	
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Weapon",meta=(AllowPrivateAccess="true"))
	class USkeletalMeshComponent* Weapon;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Billboard",meta=(AllowPrivateAccess="true"))
	class UBillboardComponent* Billboard;
	
};
