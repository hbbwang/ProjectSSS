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

	LeftHandTran = CreateDefaultSubobject<USceneComponent>(TEXT("LeftHandTran"));
	LeftHandTran->SetupAttachment(Weapon);
	
	RightHandTran = CreateDefaultSubobject<USceneComponent>(TEXT("RightHandTran"));
	RightHandTran->SetupAttachment(Weapon);
	
	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard(Editor Only)"));
	Billboard->SetupAttachment(Weapon);
	Billboard->bIsEditorOnly = true;
	Billboard->bIsScreenSizeScaled = true;
	Billboard->SetRelativeScale3D(FVector(0.75f,0.75f,0.75f));
	
}

FVector ATPWeaponBase::GetInteractiveLocation()
{
	return Weapon->GetComponentLocation();
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

