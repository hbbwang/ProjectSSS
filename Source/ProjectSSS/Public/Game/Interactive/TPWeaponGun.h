// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/Interactive//TPWeaponBase.h"
#include "TPWeaponGun.generated.h"

/**
 * 
 */

UENUM()
enum EWeaponFiringMode
{
	WFM_Single		UMETA(DisplayName="半自动"),
	WFM_Triple 		UMETA(DisplayName="三连发"),
	WFM_Automatic	UMETA(DisplayName="全自动"),
};

UENUM()
enum EWeaponReloadingMode
{
	FM_Magazine 		UMETA(DisplayName="正常填充弹夹"),
	FM_Single			UMETA(DisplayName="每次填充一发"),
};

UCLASS()
class PROJECTSSS_API ATPWeaponGun : public ATPWeaponBase
{
	GENERATED_BODY()

public:
	ATPWeaponGun();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Gun")
	TEnumAsByte<EWeaponFiringMode> CurrentWeaponFiringMode;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Gun")
	TEnumAsByte<EWeaponReloadingMode> WeaponReloadingMode;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void OnConstruction(const FTransform& Transform) override;

	
	
};
