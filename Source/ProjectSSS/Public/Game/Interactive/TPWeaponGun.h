// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/Interactive//TPWeaponBase.h"
#include "TPWeaponGun.generated.h"

/**
 * 
 */

UCLASS()
class PROJECTSSS_API ATPWeaponGun : public ATPWeaponBase
{
	GENERATED_BODY()

public:
	ATPWeaponGun();
	
	virtual void UnEquip(ATPWeaponBase* newWeapon)override;
	
	virtual void PickUp(class ATPCharacterBase* weaponOwner)override;

	virtual void AttachWeaponToCharacter(bool bLeft = false)override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void OnConstruction(const FTransform& Transform) override;

#if WITH_EDITOR
	virtual void Tick(float DeltaSeconds) override;
#endif

	virtual void Drop(FVector dropLocation, FRotator Rot)override;
	
	//开枪动画
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Animation")
	UAnimSequence* Fire;

	//重新填装
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Animation")
	UAnimSequence* Reload;

	//上膛
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Animation")
	UAnimSequence* Load;

	FTimerHandle EquipTimer;
	
	FTimerHandle PackUpTimer;

	void ClearAnimMotageTimer();

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Weapon")
	bool bOnBack;
	
};
