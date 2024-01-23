// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPBullet.generated.h"

UCLASS()
class PROJECTSSS_API ATPBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPBullet();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Bullet")
	FVector BeginPos;
	
	//有效射程
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Bullet")
	float StartGravityDistance;

	//重力增加速度
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Bullet")
	float GravityAdditiveSpeed;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Component")
	class UBoxComponent* RootCollision;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Component")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Component")
	class UProjectileMovementComponent* ProjectileMovement;
	
	UFUNCTION()
	void CollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void CollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
