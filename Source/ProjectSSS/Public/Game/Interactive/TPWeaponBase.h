// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/Interactive/TPInteractiveActor.h"
#include "TPWeaponBase.generated.h"

UCLASS()
class PROJECTSSS_API ATPWeaponBase : public ATPInteractiveActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPWeaponBase();

	inline class USkeletalMeshComponent*  GetWeaponComp()const{return Weapon;}

	UFUNCTION(BlueprintCallable,Category="Interactive")
	virtual FVector GetInteractiveLocation() override;

	UFUNCTION(BlueprintCallable)
	bool IsWeaponActive()const
	{
		return bWeaponActive;
	}

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon",meta=(ToolTip="武器在手上 的位置和旋转"))
	FTransform WeaponEquipTransform;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon",meta=(ToolTip="左后背 武器位置和旋转"))
	FTransform WeaponBackTransform_Left;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon",meta=(ToolTip="右后背 武器位置和旋转"))
	FTransform WeaponBackTransform_Right;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon",meta=(ToolTip="是否开启 左手 手腕位置和旋转变换"))
	bool bEnableHandPosRot_Left;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon",meta=(ToolTip="是否开启 右手 手腕位置和旋转变换"))
	bool bEnableHandPosRot_Right;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon",meta=(ToolTip="左手 手肘朝向的位置"))
	FVector JointTargetLocation_LeftHand;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon",meta=(ToolTip="右手 手肘朝向的位置"))
	FVector JointTargetLocation_RightHand;

	virtual void AttachWeaponToCharacter(bool bLeft = false){}
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual  void OnConstruction(const FTransform& Transform) override;

	virtual void Destroyed() override;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Weapon",meta=(AllowPrivateAccess="true"))
	bool bWeaponActive;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Weapon",meta=(AllowPrivateAccess="true"))
	class USceneComponent* Root;
	
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Weapon",meta=(AllowPrivateAccess="true"))
	class USkeletalMeshComponent* Weapon;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Weapon",meta=(AllowPrivateAccess="true"))
	class USceneComponent* LeftHandTran;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Weapon",meta=(AllowPrivateAccess="true"))
	class USceneComponent* RightHandTran;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Weapon",meta=(AllowPrivateAccess="true"))
	class USceneComponent* LeftHandTran_Aim;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Weapon",meta=(AllowPrivateAccess="true"))
	class USceneComponent* RightHandTran_Aim;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Billboard",meta=(AllowPrivateAccess="true"))
	class UBillboardComponent* Billboard;

	class UTPWorldSubsystem* WorldSubsystem;
	
};
