// Fill out your copyright notice in the Description page of Project Settings.
#include "Game/Interactive/TPWeaponBase.h"
#include "GameFramework/Character.h"
#include "Components/BillboardComponent.h"
#include "Game/TPWorldManager.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
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

	Weapon->BodyInstance.bLockXRotation = 1;
	Weapon->BodyInstance.bLockYRotation = 1;
	Weapon->BodyInstance.bLockZRotation = 1;
	Weapon->BodyInstance.bLockXTranslation = 1;
	Weapon->BodyInstance.bLockYTranslation = 1;
	
	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard(Editor Only)"));
	Billboard->SetupAttachment(Weapon);
	Billboard->bIsEditorOnly = true;
	Billboard->bIsScreenSizeScaled = true;
	Billboard->SetRelativeScale3D(FVector(0.75f,0.75f,0.75f));
	
	WeaponEquipTransform.SetLocation(FVector(-22.9,-3.65,4.5));
	WeaponEquipTransform.SetRotation(FRotator(-10.0f,112,5.0).Quaternion());
	
	WeaponBackTransform_Left.SetLocation(FVector(-8,-17,-10));
	WeaponBackTransform_Left.SetRotation(FRotator(0.0f,100.0f,0.0).Quaternion());

	WeaponBackTransform_Right.SetLocation(FVector(-8,17,-10));
	WeaponBackTransform_Right.SetRotation(FRotator(0.0f,100.0f,0.0).Quaternion());
	
}

FVector ATPWeaponBase::GetInteractiveLocation()
{
	return Weapon->GetComponentLocation();
}
PRAGMA_DISABLE_OPTIMIZATION
void ATPWeaponBase::Equip(class ATPCharacterBase* weaponOwner)
{
	//to hand
	Weapon->AttachToComponent(weaponOwner->GetMesh(),FAttachmentTransformRules::KeepWorldTransform,TEXT("RHand"));
	Weapon->SetRelativeTransform(WeaponEquipTransform);
	this->SetActorHiddenInGame(false);
	//play equip anim
	float AnimLength= 0.001f;
	if(weaponOwner->Rifle_Equip)
	{
		AnimLength =  weaponOwner->Rifle_Equip->GetPlayLength();
		weaponOwner->PlayAnimMontage( weaponOwner->Rifle_Equip );
	}
	GetWorld()->GetTimerManager().SetTimer(EquipTimer,[this]()
	{
		bWeaponActive = true;
	},AnimLength , false);
}

void ATPWeaponBase::UnEquip(ATPWeaponBase* newWeapon)
{
	//to back
	bWeaponActive = false;
	//play packUp anim
	float AnimLength= 0.001f;
	if(InteractiveOwner->Rifle_PackUp)
	{
		AnimLength =  InteractiveOwner->Rifle_PackUp->GetPlayLength();
		InteractiveOwner->PlayAnimMontage( InteractiveOwner->Rifle_PackUp );
	}
	GetWorld()->GetTimerManager().SetTimer(PackUpTimer,[this,newWeapon]()
	{
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
		if(newWeapon)
		{
			newWeapon->PickUp(InteractiveOwner);
		}
	},AnimLength , false);
}

void ATPWeaponBase::PickUp(class ATPCharacterBase* weaponOwner)
{
	//Set owner
	SetInteractiveOwner(weaponOwner);
	//Change weapon state
	Weapon->SetSimulatePhysics(false);
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->SetActorHiddenInGame(false);
	//Play pick up anims and effects...
	Equip(weaponOwner);
}

void ATPWeaponBase::Drop(FVector dropLocation , FRotator Rot)
{
	//Change weapon state
	Weapon->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);	Weapon->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	Weapon->SetWorldLocation(dropLocation);
	Weapon->SetWorldRotation(Rot);
	Weapon->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Weapon->SetSimulatePhysics(true);
	this->SetActorHiddenInGame(false);
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

