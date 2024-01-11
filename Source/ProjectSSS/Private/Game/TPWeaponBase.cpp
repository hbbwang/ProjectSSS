// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TPWeaponBase.h"

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
	Weapon->SetCollisionProfileName(TEXT("WeaponActor"));
	
	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard(Editor Only)"));
	Billboard->SetupAttachment(Weapon);
	Billboard->bIsEditorOnly = true;
	Billboard->bIsScreenSizeScaled = true;
	Billboard->SetRelativeScale3D(FVector(0.75f,0.75f,0.75f));
	
	bEquip = false;
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
	WorldSubsystem->GetWorldManager()->Weapons.Remove(this);
}

