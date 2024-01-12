// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Interactive/TPWeaponGun.h"

ATPWeaponGun::ATPWeaponGun():ATPWeaponBase()
{
	CurrentWeaponFiringMode = EWeaponFiringMode::WFM_Automatic;
	WeaponReloadingMode = EWeaponReloadingMode::FM_Magazine;
	
}

void ATPWeaponGun::BeginPlay()
{
	Super::BeginPlay();
}

void ATPWeaponGun::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}
