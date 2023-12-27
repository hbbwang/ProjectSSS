// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/TPCharacterBase.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerInput.h"
// Sets default values
ATPCharacterBase::ATPCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_playerCameraSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("PlayerCameraSpringArm"));
	_playerCameraSpringArmComp->SetupAttachment(RootComponent);
	_playerCameraSpringArmComp->bUsePawnControlRotation = true;
	
	_playerCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	_playerCameraComp->SetupAttachment(_playerCameraSpringArmComp);
	
}

void ATPCharacterBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	auto newLocation = GetMesh()->GetRelativeLocation();
	newLocation.Z = -GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	GetMesh()->SetRelativeLocation(newLocation);
	auto newRotation = GetMesh()->GetRelativeRotation();
	newRotation.Yaw = -90.0f;
	GetMesh()->SetRelativeRotation(newRotation);
}

// Called when the game starts or when spawned
void ATPCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(_inputMappingContext, 0);
		}
	}
	_socketOffset_Origin = _playerCameraSpringArmComp->SocketOffset;
}

void ATPCharacterBase::InputEvent_MoveForward(const FInputActionValue& value)
{
	auto moveVector = value.Get<float>();
	if(moveVector > 0.025f || moveVector < -0.025f )
	{
		AddMovementInput( _playerCameraComp->GetForwardVector()*FVector(1,1,0).Normalize(), moveVector);
	}
}

void ATPCharacterBase::InputEvent_MoveRightward(const FInputActionValue& value)
{
	auto moveVector = value.Get<float>();
	if(moveVector > 0.025f || moveVector < -0.025f )
	{
		AddMovementInput( _playerCameraComp->GetRightVector()*FVector(1,1,0).Normalize(), moveVector);
	}
}

void ATPCharacterBase::InputEvent_LookAxis2D(const FInputActionValue& value)
{
	auto moveVector = value.Get<FVector2D>();
	AddControllerYawInput(moveVector.X);
	AddControllerPitchInput(-moveVector.Y);
	GEngine->AddOnScreenDebugMessage(0,0,FColor::Blue,moveVector.ToString());
}

// Called every frame
void ATPCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATPCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* enhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	//Move
	enhancedInputComp->BindAction(_inputMoveForward,ETriggerEvent::Triggered,this,&ATPCharacterBase::InputEvent_MoveForward);
	enhancedInputComp->BindAction(_inputMoveBackward,ETriggerEvent::Triggered,this,&ATPCharacterBase::InputEvent_MoveForward);
	enhancedInputComp->BindAction(_inputMoveRight,ETriggerEvent::Triggered,this,&ATPCharacterBase::InputEvent_MoveRightward);
	enhancedInputComp->BindAction(_inputMoveLeft,ETriggerEvent::Triggered,this,&ATPCharacterBase::InputEvent_MoveRightward);
	//Look at
	enhancedInputComp->BindAction(_inputLookAxis2D,ETriggerEvent::Triggered,this,&ATPCharacterBase::InputEvent_LookAxis2D);
	
	// //动态修改按键映射模板
	// {
	// 	_inputMappingContext->UnmapAllKeysFromAction(_inputMove);//删除映射
	// 	auto mapping = _inputMappingContext->MapKey(_inputMove,EKeys::F);//添加新的按键映射
	// 	//mapping. //设置映射属性
	// }
}

