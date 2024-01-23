// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Interactive/TPBullet.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ATPBullet::ATPBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RootCollision"));
	SetRootComponent(RootCollision);
	RootCollision->OnComponentBeginOverlap.RemoveDynamic(this,&ATPBullet::CollisionBeginOverlap);
	RootCollision->OnComponentEndOverlap.RemoveDynamic(this,&ATPBullet::CollisionEndOverlap);
	RootCollision->OnComponentBeginOverlap.AddDynamic(this,&ATPBullet::CollisionBeginOverlap);
	RootCollision->OnComponentEndOverlap.AddDynamic(this,&ATPBullet::CollisionEndOverlap);
	RootCollision->SetGenerateOverlapEvents(true);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	Mesh->SetupAttachment(RootCollision);
	Mesh->SetCollisionProfileName("NoCollision");
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetReceivesDecals(false);
	Mesh->SetGenerateOverlapEvents(false);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	
	InitialLifeSpan = 5.0f;
}

// Called when the game starts or when spawned
void ATPBullet::BeginPlay()
{
	Super::BeginPlay();
	BeginPos = GetActorLocation();
}

void ATPBullet::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(ProjectileMovement->ProjectileGravityScale < 1.0f)
	{
		if(FVector::Dist(GetActorLocation(),BeginPos) >= StartGravityDistance * 100.0f)
		{
			ProjectileMovement->ProjectileGravityScale += DeltaSeconds * GravityAdditiveSpeed * ProjectileMovement->Velocity.Length() * 0.01f;
		}
	}
	else
	{
		PrimaryActorTick.bCanEverTick = false;
	}
}

void ATPBullet::CollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	
}

void ATPBullet::CollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	
}

