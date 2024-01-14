// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/TPCharacterBase.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/TPWorldManager.h"
#include "Game/Interactive/TPWeaponBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerInput.h"
#include "Subsystem/TPWorldSubsystem.h"

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

	InteractiveBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractiveBox"));
	InteractiveBox->SetupAttachment(GetMesh());
	InteractiveBox->SetBoxExtent(FVector(100,100,GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 1.1f));
	InteractiveBox->SetCollisionProfileName("Interactive");
	InteractiveBox->OnComponentBeginOverlap.RemoveDynamic(this,&ATPCharacterBase::InteractiveBeginOverlap);
	InteractiveBox->OnComponentBeginOverlap.AddDynamic(this,&ATPCharacterBase::InteractiveBeginOverlap);
	InteractiveBox->OnComponentEndOverlap.RemoveDynamic(this,&ATPCharacterBase::InteractiveEndOverlap);
	InteractiveBox->OnComponentEndOverlap.AddDynamic(this,&ATPCharacterBase::InteractiveEndOverlap);
	
	InteractiveBox->SetRelativeLocation(FVector(0,0,GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
		
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator( 0,0,270.0f );

	CharacterState = ECharacterState::CharacterState_Based;

	MaxWeapon = 2;
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
	
	WorldSubsystem = GetWorld()->GetSubsystem<UTPWorldSubsystem>();
}

void ATPCharacterBase::InputEvent_MoveForward(const FInputActionValue& value)
{
	auto moveVector = value.Get<float>();
	MoveAxisTarget.Y = moveVector;
	if(moveVector > 0.025f || moveVector < -0.025f )
	{
		FRotator rot = FRotator(0,GetControlRotation().Yaw,0);
		AddMovementInput( rot.Vector() , moveVector);
		bMoveInputY = true;
	}
	else
	{
		bMoveInputY = false;
	}
}

void ATPCharacterBase::InputEvent_Backward(const FInputActionValue& value)
{
	auto moveVector = value.Get<float>();
	MoveAxisTarget.Y = moveVector;
	if(moveVector > 0.025f || moveVector < -0.025f )
	{
		FRotator rot = FRotator(0,GetControlRotation().Yaw,0);
		AddMovementInput( rot.Vector() , moveVector);
		bMoveInputFlipY = true;
	}
	else
	{
		bMoveInputFlipY = false;
	}
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
		bMoveInputX = true;
	}
	else
	{
		bMoveInputX = false;
	}
}

void ATPCharacterBase::InputEvent_MoveLeftward(const FInputActionValue& value)
{
	auto moveVector = value.Get<float>();
	MoveAxisTarget.X = moveVector;
	if(moveVector > 0.025f || moveVector < -0.025f )
	{
		//AddMovementInput( _playerCameraComp->GetRightVector()*FVector(1,1,0).Normalize(), moveVector);
		FRotator rot = FRotator(0,GetControlRotation().Yaw,0);
		AddMovementInput( FRotationMatrix(rot).GetScaledAxis(EAxis::Y) , moveVector);
		bMoveInputFlipX = true;
	}
	else
	{
		bMoveInputFlipX = false;
	}
}

void ATPCharacterBase::InputEvent_Run(const FInputActionValue& value)
{
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

void ATPCharacterBase::InputEvent_Interactive(const FInputActionValue& value)
{
	auto bTrigger = value.Get<bool>();
	Interactive();
	if(InteractiveInputTrigger.IsBound())
	{
		InteractiveInputTrigger.Broadcast();
	}
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

	CurrentMoveSpeed = GetCharacterMovement()->Velocity.Length();
}

// Called to bind functionality to input
void ATPCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* enhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	//Move
	enhancedInputComp->BindAction(InputMoveForward,ETriggerEvent::Triggered,this,&ATPCharacterBase::InputEvent_MoveForward);
	enhancedInputComp->BindAction(InputMoveBackward,ETriggerEvent::Triggered,this,&ATPCharacterBase::InputEvent_Backward);
	enhancedInputComp->BindAction(InputMoveRight,ETriggerEvent::Triggered,this,&ATPCharacterBase::InputEvent_MoveRightward);
	enhancedInputComp->BindAction(InputMoveLeft,ETriggerEvent::Triggered,this,&ATPCharacterBase::InputEvent_MoveLeftward);

	enhancedInputComp->BindAction(InputMoveForward,ETriggerEvent::Completed,this,&ATPCharacterBase::InputEvent_MoveForward);
	enhancedInputComp->BindAction(InputMoveBackward,ETriggerEvent::Completed,this,&ATPCharacterBase::InputEvent_Backward);
	enhancedInputComp->BindAction(InputMoveRight,ETriggerEvent::Completed,this,&ATPCharacterBase::InputEvent_MoveRightward);
	enhancedInputComp->BindAction(InputMoveLeft,ETriggerEvent::Completed,this,&ATPCharacterBase::InputEvent_MoveLeftward);
	//Look at
	enhancedInputComp->BindAction(InputLookAxis2D,ETriggerEvent::Triggered,this,&ATPCharacterBase::InputEvent_LookAxis2D);
	//Run
	enhancedInputComp->BindAction(InputRun,ETriggerEvent::Triggered,this,&ATPCharacterBase::InputEvent_Run);
	enhancedInputComp->BindAction(InputRun,ETriggerEvent::Completed,this,&ATPCharacterBase::InputEvent_Run);
	//Interactive
	enhancedInputComp->BindAction(InputInteractive,ETriggerEvent::Started,this,&ATPCharacterBase::InputEvent_Interactive);
	
	// //动态修改按键映射模板
	// {
	// 	_inputMappingContext->UnmapAllKeysFromAction(_inputMove);//删除映射
	// 	auto mapping = _inputMappingContext->MapKey(_inputMove,EKeys::F);//添加新的按键映射
	// 	//mapping. //设置映射属性
	// }
}

void ATPCharacterBase::InteractiveBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(0,10,FColor::Red,OtherActor->GetActorLabel());
	auto interactiveActor = Cast<ATPInteractiveActor>(OtherActor);
	if(interactiveActor)
	{
		InteractiveActors.AddUnique(interactiveActor);
	}
}

void ATPCharacterBase::InteractiveEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto interactiveActor = Cast<ATPInteractiveActor>(OtherActor);
	if(interactiveActor)
	{
		InteractiveActors.Remove(interactiveActor);
	}	
}

void ATPCharacterBase::Interactive()
{
	float currentDis = 99999.0f;
	ATPInteractiveActor* nearestActor = nullptr;
	for(auto& i : InteractiveActors)
	{
		auto newDist = FVector::Dist(i->GetInteractiveLocation(),GetActorLocation());
		if( newDist < currentDis)
		{
			currentDis = newDist;
			nearestActor = i;
		}
	}
	if(nearestActor != nullptr)
	{
		auto newWeapon = Cast<ATPWeaponBase>(nearestActor);
		if(newWeapon && !newWeapon->GetInteractiveOwner())
		{
			newWeapon->SetInteractiveOwner(this);//lock
			newWeapon->GetWeaponComp()->SetSimulatePhysics(false);
			newWeapon->SetActorHiddenInGame(true);
			if(Weapons.Num()>=MaxWeapon)
			{
				if(CurrentWeapon)
				{
					Weapons.Remove(CurrentWeapon);
				}
				else
				{
					CurrentWeapon = Weapons[0];
					Weapons.RemoveAt(0);
				}
				auto dropLoc = newWeapon->GetInteractiveLocation();
				dropLoc.Z += 20.0f;
				CurrentWeapon->Drop(dropLoc,newWeapon->GetWeaponComp()->GetComponentRotation());
				newWeapon->PickUp(this);
			}
			else
			{
				if(CurrentWeapon)
				{
					CurrentWeapon->UnEquip(newWeapon);
				}
				else
				{
					newWeapon->PickUp(this);
				}
			}
			Weapons.AddUnique(newWeapon);
			CurrentWeapon = newWeapon;
		}
	}
}

void ATPCharacterBase::DropWeapon(ATPWeaponBase* weapon)
{
	if(CurrentWeapon == weapon)
	{
		CurrentWeapon = nullptr;
	}
	weapon->Drop(weapon->GetInteractiveLocation(),FRotator(0,0,0));
	Weapons.Remove(weapon);
	
}

