// Fill out your copyright notice in the Description page of Project Settings.
#include "Game/Interactive/TPWeaponBase.h"
#include "GameFramework/Character.h"
#include "Components/BillboardComponent.h"
#include "Game/TPWorldManager.h"
#include "Subsystem/TPWorldSubsystem.h"

// Sets default values
ATPWeaponBase::ATPWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	SetRootComponent(Weapon);
	Weapon->SetRelativeLocation(FVector(0,0,0));
	Weapon->SetRelativeRotation(FRotator(0,0,0));
	Weapon->SetSimulatePhysics(true);
	Weapon->SetCollisionProfileName(TEXT("Interactive"));
	Weapon->SetGenerateOverlapEvents(true);
	
	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard(Editor Only)"));
	Billboard->SetupAttachment(Weapon);
	Billboard->bIsEditorOnly = true;
	Billboard->bIsScreenSizeScaled = true;
	Billboard->SetRelativeScale3D(FVector(0.75f,0.75f,0.75f));
	
	WeaponEquipTransform.SetLocation(FVector(-22.9,-3.65,4.5));
	WeaponEquipTransform.SetRotation(FRotator(-10.0f,112,5.0).Quaternion());
	
	WeaponBackTransform.SetLocation(FVector(-8,-17,-10));
	WeaponBackTransform.SetRotation(FRotator(0.0f,100.0f,0.0).Quaternion());
	
}

FVector ATPWeaponBase::GetInteractiveLocation()
{
	return Weapon->GetComponentLocation();
}

void ATPWeaponBase::Equip(class ATPCharacterBase* weaponOwner)
{
	//to hand
	bWeaponActive = true;
	Weapon->AttachToComponent(weaponOwner->GetMesh(),FAttachmentTransformRules::KeepWorldTransform,TEXT("RHand"));
	Weapon->SetRelativeTransform(WeaponEquipTransform);
}

void ATPWeaponBase::UnEquip(ATPWeaponBase* newWeapon)
{
	//to back
	bWeaponActive = false;
	auto Index = InteractiveOwner->Weapons.Find(newWeapon);
	if(Index == 0)
	{
		Weapon->AttachToComponent(InteractiveOwner->GetMesh(),FAttachmentTransformRules::KeepWorldTransform,TEXT("BackWeapon0"));
	}
	else
	{
		Weapon->AttachToComponent(InteractiveOwner->GetMesh(),FAttachmentTransformRules::KeepWorldTransform,TEXT("BackWeapon1"));
	}
	Weapon->SetRelativeTransform(WeaponBackTransform);
	if(!newWeapon)
	{
		newWeapon->Equip(InteractiveOwner);
	}
}

void ATPWeaponBase::PickUp(class ATPCharacterBase* weaponOwner)
{
	//Set owner
	SetInteractiveOwner(weaponOwner);
	//Change weapon state
	Weapon->SetSimulatePhysics(false);
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Play pick up anims and effects...
	Equip(weaponOwner);
}

void ATPWeaponBase::Drop(FVector dropLocation)
{
	//Play drop anims and effects...
	
	//Change weapon state
	Weapon->SetSimulatePhysics(false);
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Weapon->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	//Set owner
	SetInteractiveOwner(nullptr);
}

// Called when the game starts or when spawned
void ATPWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void ATPWeaponBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	auto billboardTex = LoadObject<UTexture2D>(nullptr,TEXT("Game/Content/Textures/Base/T_WeaponIcon.T_WeaponIcon"));
	if(billboardTex)
	{
		Billboard->SetSprite(billboardTex);
	}
	
	WorldSubsystem  = GetWorld()->GetSubsystem<UTPWorldSubsystem>();
	if(WorldSubsystem)
	{
		WorldSubsystem->GetWorldManager()->Weapons.AddUnique(this);
	}
		
}

void ATPWeaponBase::Destroyed()
{
	Super::Destroyed();
	if(WorldSubsystem)
	{
		WorldSubsystem->GetWorldManager()->Weapons.Remove(this);
	}
}

