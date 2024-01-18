// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Interactive/TPWeaponGun.h"

ATPWeaponGun::ATPWeaponGun():ATPWeaponBase()
{
#if WITH_EDITOR
	PrimaryActorTick.bCanEverTick = true;
#endif
	CurrentWeaponFiringMode = EWeaponFiringMode::WFM_Automatic;
	WeaponReloadingMode = EWeaponReloadingMode::FM_Magazine;
	
	WeaponEquipTransform.SetLocation(FVector(-22.9,-3.65,4.5));
	WeaponEquipTransform.SetRotation(FRotator(-10.0f,112,5.0).Quaternion());
	
	WeaponBackTransform_Left.SetLocation(FVector(-8,-17,-10));
	WeaponBackTransform_Left.SetRotation(FRotator(0.0f,100.0f,0.0).Quaternion());

	WeaponBackTransform_Right.SetLocation(FVector(-8,17,-10));
	WeaponBackTransform_Right.SetRotation(FRotator(0.0f,100.0f,0.0).Quaternion());
}

void ATPWeaponGun::UnEquip(ATPWeaponBase* newWeapon)
{
	Super::UnEquip(newWeapon);
	InteractiveOwner->bPackUp = true;
	//to back
	bWeaponActive = false;
	//play packUp anim
	float AnimLength= 0.001f;
	if(InteractiveOwner->Rifle_PackUp)
	{
		AnimLength =  InteractiveOwner->Rifle_PackUp->GetPlayLength();
		InteractiveOwner->PlayAnimMontage( InteractiveOwner->Rifle_PackUp );
	}
	ClearAnimMotageTimer();
	GetWorld()->GetTimerManager().SetTimer(PackUpTimer,[this,newWeapon]()
	{
		InteractiveOwner->bPackUp = false;
		auto Index = InteractiveOwner->Weapons.Find(this);
		if(Index == 0)
		{
			Weapon->AttachToComponent(InteractiveOwner->GetMesh(),FAttachmentTransformRules::KeepWorldTransform,TEXT("BackWeapon"));
			Weapon->SetRelativeTransform(WeaponBackTransform_Left);
		}
		else
		{
			Weapon->AttachToComponent(InteractiveOwner->GetMesh(),FAttachmentTransformRules::KeepWorldTransform,TEXT("BackWeapon"));
			Weapon->SetRelativeTransform(WeaponBackTransform_Right);
		}
		bOnBack = true;
		if(newWeapon)
		{
			newWeapon->PickUp(InteractiveOwner);
		}
	},AnimLength , false);
}

void ATPWeaponGun::PickUp(ATPCharacterBase* weaponOwner)
{
	Super::PickUp(weaponOwner);
	InteractiveOwner->bEquip = true;
	//Set owner
	SetInteractiveOwner(weaponOwner);
	//Change weapon state
	Weapon->SetSimulatePhysics(false);
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->SetActorHiddenInGame(false);
	Weapon->AttachToComponent(weaponOwner->GetMesh(),FAttachmentTransformRules::KeepWorldTransform,TEXT("RHand"));
	Weapon->SetRelativeTransform(WeaponEquipTransform);
	//play equip anim
	float AnimLength= 0.001f;
	if(weaponOwner->Rifle_Equip)
	{
		AnimLength =  weaponOwner->Rifle_Equip->GetPlayLength();
		weaponOwner->PlayAnimMontage( weaponOwner->Rifle_Equip );
	}
	ClearAnimMotageTimer();
	GetWorld()->GetTimerManager().SetTimer(EquipTimer,[this]()
	{
		InteractiveOwner->bEquip = false;
		bWeaponActive = true;
		bOnBack = false;
	},AnimLength , false);
}

void ATPWeaponGun::Drop(FVector dropLocation, FRotator Rot)
{
	Super::Drop(dropLocation,Rot);
	//Change weapon state
	Weapon->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	Weapon->SetWorldLocation(dropLocation);
	Weapon->SetWorldRotation(Rot);
	Weapon->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Weapon->SetSimulatePhysics(true);
	this->SetActorHiddenInGame(false);
	//Set owner
	SetInteractiveOwner(nullptr);
	bOnBack = false;
}

void ATPWeaponGun::BeginPlay()
{
	Super::BeginPlay();
}

void ATPWeaponGun::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

#if WITH_EDITOR
void ATPWeaponGun::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
#endif

void ATPWeaponGun::ClearAnimMotageTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(EquipTimer);
	GetWorld()->GetTimerManager().ClearTimer(PackUpTimer);
}
