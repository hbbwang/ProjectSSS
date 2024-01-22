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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
