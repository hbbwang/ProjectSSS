// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "TPCharacterBase.generated.h"

class ATPWeaponBase;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRunInputTrigger, bool, bRun);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAimInputTrigger, bool, bAim);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFireInputTrigger, bool, bFire);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractiveInputTrigger);

UENUM()
enum ECharacterState
{
	CharacterState_Based		UMETA(DisplayName="基础状态"),
	CharacterState_Driver		UMETA(DisplayName="驾驶状态"),
	CharacterState_Pilot		UMETA(DisplayName="飞行状态"),
};

UCLASS()
class PROJECTSSS_API ATPCharacterBase : public ACharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ATPCharacterBase();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	float WalkSpeed;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	float RunSpeed;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	float WalkSpeedWithWeapon;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	float AimWalkSpeedWithWeapon;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	float RunSpeedWithWeapon;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Input")
	bool bRun;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Input")
	bool bRunInput;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Input")
	FVector2D MoveAxis;
	FVector2D InputMoveX;
	FVector2D InputMoveY;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Input")
	FVector2D LookAxis;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Input")
	bool bMoveInputX;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Input")
	bool bMoveInputFlipX;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Input")
	bool bMoveInputY;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Input")
	bool bMoveInputFlipY;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Input")
	float InputDeltaAngle;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Input")
	float CurrentMoveSpeed;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Input")
	float BeginWalkSpeed;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Input")
	float BeginRunSpeed;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Input")
	float StopMovementSpeed;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Input")
	bool bAim;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Input")
	bool bInputAim;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Input")
	bool bFire;
	
	float CurrentFireInterval;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Input")
	FRotator AimOffsetRot;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Input")
	FRotator AimOffsetRotBias;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	float AimOffsetRotSpeed;
	
	UPROPERTY(BlueprintAssignable,Category="Delegate")
	FRunInputTrigger RunInputTrigger;

	UPROPERTY(BlueprintAssignable,Category="Delegate")
	FAimInputTrigger AimInputTrigger;

	UPROPERTY(BlueprintAssignable,Category="Delegate")
	FFireInputTrigger FireInputTrigger;
	
	UPROPERTY(BlueprintAssignable,Category="Delegate")
	FInteractiveInputTrigger InteractiveInputTrigger;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Character)
	TEnumAsByte<ECharacterState> CharacterState;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Animation")
	UAnimMontage* Rifle_Equip;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Animation")
	UAnimMontage* Rifle_PackUp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Animation")
	bool bFlipAnimation;

	FVector LastMovementX;
	FVector LastMovementY;
	float TargetMovementSpeed;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class UTPWorldSubsystem* WorldSubsystem;

#pragma region EnhancedInput
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	class UInputAction* InputMoveForward;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	class UInputAction* InputMoveBackward;
	void InputEvent_MoveForward(const FInputActionValue& value);
	void InputEvent_Backward(const FInputActionValue& value);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	class UInputAction* InputMoveRight;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	class UInputAction* InputMoveLeft;
	void InputEvent_MoveRightward(const FInputActionValue& value);
	void InputEvent_MoveLeftward(const FInputActionValue& value);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	class UInputAction* InputRun;
	void InputEvent_Run(const FInputActionValue& value);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	class UInputAction* InputLookAxis2D;
	void InputEvent_LookAxis2D(const FInputActionValue& value);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	class UInputAction* InputInteractive;
	void InputEvent_Interactive(const FInputActionValue& value);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	class UInputAction* InputAim;
	void InputEvent_Aim(const FInputActionValue& value);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	class UInputAction* InputFire;
	void InputEvent_Fire(const FInputActionValue& value);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	class UInputAction* InputChangeAimDirection;
	void InputEvent_ChangeAimDirection(const FInputActionValue& value);

#pragma  endregion 

private:
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Component",meta=(AllowPrivateAccess="true"))
	class USpringArmComponent* PlayerCameraSpringArmComp;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Component",meta=(AllowPrivateAccess="true"))
	class UCameraComponent* PlayerCameraComp;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Component",meta=(AllowPrivateAccess="true"))
	class UBoxComponent* InteractiveBox;

	UFUNCTION()
	void InteractiveBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void InteractiveEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	TArray<class ATPInteractiveActor*> InteractiveActors;
	
	class APlayerController* PlayerController = nullptr;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable)
	void SpawnWeapon(class UClass* weaponClass);

	UFUNCTION(BlueprintCallable)
	void PickUpWeapon(AActor* weapon);
	
	UFUNCTION(BlueprintCallable)
	void DropWeapon(class ATPWeaponBase* weapon);
	
	void FlipAnimation(bool bLeft = false);

#if WITH_EDITOR
	
	UFUNCTION(BlueprintCallable,Exec)
	void HoldAimState();
	
	bool bHoldAimState;
#endif

	UFUNCTION(BlueprintCallable,Exec)
	void ChangeAimDirection(int bLeft = 0)
	{
		FlipAnimation((bool)bLeft);
	}
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Weapon")
	TArray<class ATPWeaponBase*> Weapons;

	//Current weapon on hand.
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Weapon")
	class ATPWeaponBase* CurrentWeapon;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Weapon",meta=(AllowPrivateAccess="true"))
	int32 MaxWeapon;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Weapon")
	bool bEquip;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Weapon")
	bool bPackUp;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Weapon",meta=(ToolTip="当前武器的弹丸扩散大小"))
	float CurrentWeaponBulletSpreadSize;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Weapon")
	int32 FireCount; 
	
protected:

	virtual void Interactive();

	void UpdateMovementSpeed();

	void SpawnBullet();
};
