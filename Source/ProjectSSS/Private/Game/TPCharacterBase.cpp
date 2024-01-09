// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/TPCharacterBase.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerInput.h"
// Sets default values
ATPCharacterBase::ATPCharacterBase():
	SpeedBeginWalk(FVector2D(8,8)),
	SpeedEndWalk(FVector2D(20,20)),
	SpeedBeginRun(12),
	SpeedEndRun(25)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerCameraSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("PlayerCameraSpringArm"));
	PlayerCameraSpringArmComp->SetupAttachment(RootComponent);
	PlayerCameraSpringArmComp->bUsePawnControlRotation = true;
	PlayerCameraSpringArmComp->bEnableCameraLag = true;
	PlayerCameraSpringArmComp->bEnableCameraRotationLag = true;
	PlayerCameraSpringArmComp->CameraLagSpeed = 10.0f;
	PlayerCameraSpringArmComp->CameraRotationLagSpeed = 15.0f;
	PlayerCameraSpringArmComp->CameraLagMaxDistance = 10.0f;
	PlayerCameraSpringArmComp->TargetArmLength = 120.0f;
	PlayerCameraSpringArmComp->SocketOffset = FVector(0,30.0f,80.0f);
	
	PlayerCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCameraComp->SetupAttachment(PlayerCameraSpringArmComp);
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator( 0,0,180.0f );

	CharacterState = ECharacterState::CharacterState_Based;
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
	PlayerController = Cast<APlayerController>(Controller);
	//Add Input Mapping Context
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void ATPCharacterBase::InputEvent_MoveForward(const FInputActionValue& value)
{
	auto moveVector = value.Get<float>();
	MoveAxisTarget.Y = moveVector;
	if(moveVector > 0.025f || moveVector < -0.025f )
	{
		//AddMovementInput( _playerCameraComp->GetForwardVector()*FVector(1,1,0).Normalize(), moveVector);
		FRotator rot = FRotator(0,GetControlRotation().Yaw,0);
		AddMovementInput( rot.Vector() , moveVector);
	}
	bMoveInputY = !FMath::IsNearlyEqual(moveVector,0.0f,0.01f);
}

void ATPCharacterBase::InputEvent_MoveRightward(const FInputActionValue& value)
{
	auto moveVector = value.Get<float>();
	MoveAxisTarget.X = moveVector;
	if(moveVector > 0.025f || moveVector < -0.025f )
	{
		//AddMovementInput( _playerCameraComp->GetRightVector()*FVector(1,1,0).Normalize(), moveVector);
		FRotator rot = FRotator(0,GetControlRotation().Yaw,0);
		AddMovementInput( FRotationMatrix(rot).GetScaledAxis(EAxis::Y) , moveVector);
	}
	bMoveInputX = !FMath::IsNearlyEqual(moveVector,0.0f,0.01f);
}

void ATPCharacterBase::InputEvent_Run(const FInputActionValue& value)
{
	// if(_moveAxisTarget.Y>0.001f)
	// {
	// 	_bRun = value.Get<bool>();
	// }
	// else
	// {
	// 	_bRun = false;
	// }
	bRun = value.Get<bool>();
	if(RunInputTrigger.IsBound())
	{
		RunInputTrigger.Broadcast(bRun);
	}
}

void ATPCharacterBase::InputEvent_LookAxis2D(const FInputActionValue& value)
{
	auto moveVector = value.Get<FVector2D>();
	AddControllerYawInput(moveVector.X);
	AddControllerPitchInput(-moveVector.Y);
	//GEngine->AddOnScreenDebugMessage(0,0,FColor::Blue,moveVector.ToString());
}

// Called every frame
void ATPCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float moveSpeedX = bRun?SpeedBeginRun:SpeedBeginWalk.X;
	float moveSpeedY = bRun?SpeedBeginRun:SpeedBeginWalk.Y;
	if(FMath::IsNearlyEqual(MoveAxisTarget.X ,  0.0f))
		moveSpeedX = SpeedEndWalk.X;
	if(FMath::IsNearlyEqual(MoveAxisTarget.Y ,  0.0f))
		moveSpeedY = SpeedEndWalk.Y;
	
	if(!FMath::IsNearlyEqual(MoveAxis.X,MoveAxisTarget.X))
	{
		MoveAxis.X = FMath::FInterpTo(MoveAxis.X, MoveAxisTarget.X, GetWorld()->GetDeltaSeconds(),moveSpeedX);
	}
	if(!FMath::IsNearlyEqual(MoveAxis.Y,MoveAxisTarget.Y))
	{
		MoveAxis.Y = FMath::FInterpTo(MoveAxis.Y, MoveAxisTarget.Y, GetWorld()->GetDeltaSeconds(),moveSpeedY);
	}
	
	//Compute input rot
	{
		FVector dir = FVector(MoveAxis.Y,-MoveAxis.X,0.0f);
		FRotator dir2rot = FRotationMatrix::MakeFromX(dir).Rotator();
		auto cam2actorRot = PlayerCameraComp->GetComponentRotation() - GetActorRotation();
		cam2actorRot.Normalize();
		auto result = ( cam2actorRot- dir2rot);
		result.Normalize();
		InputDeltaAngle = result.Yaw;
	}
}

// Called to bind functionality to input
void ATPCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* enhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	//Move
	enhancedInputComp->BindAction(InputMoveForward,ETriggerEvent::Triggered,this,&ATPCharacterBase::InputEvent_MoveForward);
	enhancedInputComp->BindAction(InputMoveBackward,ETriggerEvent::Triggered,this,&ATPCharacterBase::InputEvent_MoveForward);
	enhancedInputComp->BindAction(InputMoveRight,ETriggerEvent::Triggered,this,&ATPCharacterBase::InputEvent_MoveRightward);
	enhancedInputComp->BindAction(InputMoveLeft,ETriggerEvent::Triggered,this,&ATPCharacterBase::InputEvent_MoveRightward);

	enhancedInputComp->BindAction(InputMoveForward,ETriggerEvent::Completed,this,&ATPCharacterBase::InputEvent_MoveForward);
	enhancedInputComp->BindAction(InputMoveBackward,ETriggerEvent::Completed,this,&ATPCharacterBase::InputEvent_MoveForward);
	enhancedInputComp->BindAction(InputMoveRight,ETriggerEvent::Completed,this,&ATPCharacterBase::InputEvent_MoveRightward);
	enhancedInputComp->BindAction(InputMoveLeft,ETriggerEvent::Completed,this,&ATPCharacterBase::InputEvent_MoveRightward);
	//Look at
	enhancedInputComp->BindAction(InputLookAxis2D,ETriggerEvent::Triggered,this,&ATPCharacterBase::InputEvent_LookAxis2D);
	//Run
	enhancedInputComp->BindAction(InputRun,ETriggerEvent::Triggered,this,&ATPCharacterBase::InputEvent_Run);
	enhancedInputComp->BindAction(InputRun,ETriggerEvent::Completed,this,&ATPCharacterBase::InputEvent_Run);
	
	// //动态修改按键映射模板
	// {
	// 	_inputMappingContext->UnmapAllKeysFromAction(_inputMove);//删除映射
	// 	auto mapping = _inputMappingContext->MapKey(_inputMove,EKeys::F);//添加新的按键映射
	// 	//mapping. //设置映射属性
	// }
}

