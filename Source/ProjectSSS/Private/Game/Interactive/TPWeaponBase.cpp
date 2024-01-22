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

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(Root);
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(Root);
	Weapon->SetRelativeLocation(FVector(0,0,0));
	Weapon->SetRelativeRotation(FRotator(0,0,0));
	Weapon->SetSimulatePhysics(true);
	Weapon->SetCollisionProfileName(TEXT("Interactive"));
	Weapon->SetGenerateOverlapEvents(true);
	Weapon->SetReceivesDecals(false);
	
	Weapon->BodyInstance.bLockXRotation = 1;
	Weapon->BodyInstance.bLockYRotation = 1;
	Weapon->BodyInstance.bLockZRotation = 1;
	Weapon->BodyInstance.bLockXTranslation = 1;
	Weapon->BodyInstance.bLockYTranslation = 1;

	LeftHandTran = CreateDefaultSubobject<USceneComponent>(TEXT("LeftHandTran"));
	LeftHandTran->SetupAttachment(Root);
	
	RightHandTran = CreateDefaultSubobject<USceneComponent>(TEXT("RightHandTran"));
	RightHandTran->SetupAttachment(Root);
	RightHandTran->SetRelativeRotation(FRotator(0,-90,0));

	LeftHandTran_Aim = CreateDefaultSubobject<USceneComponent>(TEXT("LeftHandTran_Aim"));
	LeftHandTran_Aim->SetupAttachment(Root);
	
	RightHandTran_Aim = CreateDefaultSubobject<USceneComponent>(TEXT("RightHandTran_Aim"));
	RightHandTran_Aim->SetupAttachment(Root);
	RightHandTran_Aim->SetRelativeRotation(FRotator(0,-90,0));
	
	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard(Editor Only)"));
	Billboard->SetupAttachment(Weapon);
	Billboard->bIsEditorOnly = true;
	Billboard->bIsScreenSizeScaled = true;
	Billboard->SetRelativeScale3D(FVector(0.75f,0.75f,0.75f));

	BulletCount = 1;
	MinBulletSpreadSize = 0.0f;
	BulletSpreadAdditiveSize = 1.0f;
	FireInterval = 0.15f;
	BulletSpreadSubtractSpeed = 3.0f;
}

FVector ATPWeaponBase::GetInteractiveLocation()
{
	return Weapon->GetComponentLocation();
}

// Called when the game starts or when spawned
void ATPWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	if(WorldSubsystem)
	{
		WorldSubsystem->GetWorldManager()->Weapons.AddUnique(this);
	}
}

void ATPWeaponBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if(WorldSubsystem)
    {
    	WorldSubsystem->GetWorldManager()->Weapons.Remove(this);
    }
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
}

void ATPWeaponBase::Destroyed()
{
	Super::Destroyed();
	if(WorldSubsystem)
	{
		WorldSubsystem->GetWorldManager()->Weapons.Remove(this);
	}
}

