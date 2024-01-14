// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Interactive/TPInteractiveActor.h"

// Sets default values
ATPInteractiveActor::ATPInteractiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ATPInteractiveActor::UnEquip(ATPWeaponBase* newWeapon)
{
	
}

void ATPInteractiveActor::PickUp(ATPCharacterBase* weaponOwner)
{
	
}

void ATPInteractiveActor::Drop(FVector dropLocation, FRotator Rot)
{
	
}
