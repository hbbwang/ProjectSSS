// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Interactive/TPBullet.h"

#include "KismetTraceUtils.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#if WITH_EDITOR
bool ATPBullet::bBulletLineTraceDebug;
#endif

// Sets default values
ATPBullet::ATPBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RootCollision"));
	SetRootComponent(RootCollision);
	// RootCollision->OnComponentBeginOverlap.RemoveDynamic(this,&ATPBullet::CollisionBeginOverlap);
	// RootCollision->OnComponentEndOverlap.RemoveDynamic(this,&ATPBullet::CollisionEndOverlap);
	// RootCollision->OnComponentBeginOverlap.AddDynamic(this,&ATPBullet::CollisionBeginOverlap);
	// RootCollision->OnComponentEndOverlap.AddDynamic(this,&ATPBullet::CollisionEndOverlap);
	RootCollision->SetGenerateOverlapEvents(false);
	RootCollision->SetUseCCD(false);
	//我们通过上下帧使用Sweep进行碰撞
	RootCollision->SetCollisionProfileName("NoCollision");
	RootCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	Mesh->SetupAttachment(RootCollision);
	Mesh->SetCollisionProfileName("NoCollision");
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetReceivesDecals(false);
	Mesh->SetGenerateOverlapEvents(false);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bSweepCollision = 0;
	
	InitialLifeSpan = 5.0f;
	BulletSizeChange = 50.0f;
}

// Called when the game starts or when spawned
void ATPBullet::BeginPlay()
{
	Super::BeginPlay();
	BeginPos = GetActorLocation();
	LastPos = GetActorLocation();
}

void ATPBullet::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	auto dis = FVector::Dist(GetActorLocation(),BeginPos);
	
	if(ProjectileMovement->ProjectileGravityScale < 1.0f)
	{
		if(dis >= StartGravityDistance * 100.0f)
		{
			ProjectileMovement->ProjectileGravityScale += DeltaSeconds * GravityAdditiveSpeed * ProjectileMovement->Velocity.Length() * 0.01f;
		}
	}
	
	if(GetOwner()->GetInstigatorController()->IsLocalController() && dis > 1000.0f)
	{
		auto scale3D = GetActorScale3D();
		scale3D += FVector(0,DeltaSeconds * BulletSizeChange,DeltaSeconds * BulletSizeChange);
		SetActorScale3D(scale3D);
	}

	float CurrentDamage = 30;
	if(DamageAttenuation)
	{
		CurrentDamage = DamageAttenuation->GetFloatValue(dis);
	}
	
	FVector newPos = GetActorLocation();
	TArray<FHitResult> hitResults;
	FCollisionObjectQueryParams objectQueryParams;
	objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);
	objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Vehicle);
	FCollisionQueryParams queryParams = FCollisionQueryParams::DefaultQueryParam;
	queryParams.AddIgnoredActor(this);
	TArray<AActor*> attachActors;
	this->GetAttachedActors(attachActors);
	queryParams.AddIgnoredActors(attachActors);
	queryParams.bTraceComplex = true;//接触复杂碰撞
	bool bHit = GetWorld()->LineTraceMultiByObjectType(hitResults,LastPos,newPos,objectQueryParams,queryParams);
	#if WITH_EDITOR
	if(bBulletLineTraceDebug)
	{
		FHitResult lresult;
		DrawDebugLineTraceSingle(GetWorld(),
			LastPos,
			newPos,
			EDrawDebugTrace::Type::ForDuration,
			bHit, lresult, FLinearColor::Red, FLinearColor::Green, 2.0f);
	}
	#endif
	if(bHit)
	{
		for(auto& hitResult : hitResults)
		{
			auto actor = hitResult.GetActor();
			auto comp = hitResult.GetComponent();
			if(actor != GetOwner() && actor && comp)
			{
				if(!actor->IsA<ATPBullet>())
				{
					#if WITH_EDITOR
					UE_LOG(LogTemp,Log,TEXT("Bullet Hit : %s"),*actor->GetName());		
					#endif
					//Set Physics moving.
					if(comp->IsSimulatingPhysics())
					{
						comp->AddImpulseAtLocation(ProjectileMovement->Velocity * BulletPower,hitResult.Location,hitResult.BoneName);
					}
					//Set Damage
				
					//
					this->Destroy();
				}
			}
		}
	}
	LastPos = newPos;
}

// void ATPBullet::CollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
//                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {

// }
//
// void ATPBullet::CollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
// 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
// {
//
// 	
// }

