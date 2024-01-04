// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "TPCharacterBase.generated.h"

UCLASS()
class PROJECTSSS_API ATPCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPCharacterBase();

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Input")
	FVector2D _speed_beginWalk;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Input")
	FVector2D _speed_endWalk;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Input")
	float _speed_beginRun;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Input")
	float _speed_endRun;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Input")
	bool _bRun;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Input")
	FVector2D _moveAxis;
	FVector2D _moveAxisTarget;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Input")
	bool _bMoveInputX;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Input")
	bool _bMoveInputY;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Input")
	float _inputDeltaAngle;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Component")
	FVector _socketOffset_Origin;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region EnhancedInput
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	class UInputMappingContext* _inputMappingContext;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	class UInputAction* _inputMoveForward;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	class UInputAction* _inputMoveBackward;
	void InputEvent_MoveForward(const FInputActionValue& value);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	class UInputAction* _inputMoveRight;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	class UInputAction* _inputMoveLeft;
	void InputEvent_MoveRightward(const FInputActionValue& value);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	class UInputAction* _inputRun;
	void InputEvent_Run(const FInputActionValue& value);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	class UInputAction* _inputLookAxis2D;
	void InputEvent_LookAxis2D(const FInputActionValue& value);

#pragma  endregion 

private:
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Component",meta=(AllowPrivateAccess="true"))
	class USpringArmComponent* _playerCameraSpringArmComp;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Component",meta=(AllowPrivateAccess="true"))
	class UCameraComponent* _playerCameraComp;

	class APlayerController* _playerController = nullptr;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnConstruction(const FTransform& Transform) override;

};
