// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/TPCharacterBase.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "KismetTraceUtils.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/TPWorldManager.h"
#include "Game/Interactive/TPBullet.h"
#include "Game/Interactive/TPWeaponBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Subsystem/TPWorldSubsystem.h"

// Sets default values
ATPCharacterBase::ATPCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetReceivesDecals(false);
	
	PlayerCameraSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("PlayerCameraSpringArm"));
	PlayerCameraSpringArmComp->SetupAttachment(RootComponent);
	PlayerCameraSpringArmComp->bUsePawnControlRotation = true;
	PlayerCameraSpringArmComp->bEnableCameraLag = true;
	PlayerCameraSpringArmComp->bEnableCameraRotationLag = true;
	PlayerCameraSpringArmComp->CameraLagSpeed = 10.0f;
	PlayerCameraSpringArmComp->CameraRotationLagSpeed = 15.0f;
	PlayerCameraSpringArmComp->CameraLagMaxDistance = 10.0f;
	PlayerCameraSpringArmComp->TargetArmLength = 120.0f;
	FlipAnimation(bFlipAnimation);
	
	PlayerCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCameraComp->SetupAttachment(PlayerCameraSpringArmComp);
	bUseControllerRotationYaw = false;

	InteractiveBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractiveBox"));
	InteractiveBox->SetupAttachment(GetMesh());
	InteractiveBox->SetBoxExtent(FVector(100,100,GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 1.1f));
	//InteractiveBox->SetCollisionProfileName("Interactive");
	InteractiveBox->OnComponentBeginOverlap.RemoveDynamic(this,&ATPCharacterBase::InteractiveBeginOverlap);
	InteractiveBox->OnComponentBeginOverlap.AddDynamic(this,&ATPCharacterBase::InteractiveBeginOverlap);
	InteractiveBox->OnComponentEndOverlap.RemoveDynamic(this,&ATPCharacterBase::InteractiveEndOverlap);
	InteractiveBox->OnComponentEndOverlap.AddDynamic(this,&ATPCharacterBase::InteractiveEndOverlap);
	
	InteractiveBox->SetRelativeLocation(FVector(0,0,GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
		
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator( 0,210.0f,0 );

	CharacterState = ECharacterState::CharacterState_Based;
	AimOffsetRotSpeed = 10.0f;
	AimOffsetRotBias = FRotator(0,0,0);
	MaxWeapon = 2;

	WalkSpeed = 300.0f;
	RunSpeed = 500.0f;
	WalkSpeedWithWeapon = 250.0f;
	RunSpeedWithWeapon = 450.0f;
	AimWalkSpeedWithWeapon = 100.0f;
	StopMovementSpeed = 600.0f;
	BeginWalkSpeed = 2.5f;
	BeginRunSpeed = 4.5f;
	TargetMovementSpeed = 0;
	
	bFlipAnimation = false;
	FireCount = -1;
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

void ATPCharacterBase::SpawnWeapon(UClass* weaponClass)
{
	auto newWeapon = GetWorld()->SpawnActor(weaponClass);
	newWeapon->SetActorLocation(this->GetActorLocation());
	
	PickUpWeapon(newWeapon);
}

// Called when the game starts or when spawned
void ATPCharacterBase::BeginPlay()
{
	FlipAnimation(bFlipAnimation);
	
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

	TargetMovementSpeed = 0;
	
	Super::BeginPlay();
}

void ATPCharacterBase::InputEvent_MoveForward(const FInputActionValue& value)
{
	auto moveVector = value.Get<float>();
	InputMoveY.X = moveVector;
	if(moveVector > 0.025f || moveVector < -0.025f )
	{
		FRotator rot = FRotator(0,GetControlRotation().Yaw,0);
		LastMovementY = rot.Vector() ;
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
	InputMoveY.Y = moveVector;
	if(moveVector < -0.005f )
	{
		FRotator rot = FRotator(0,GetControlRotation().Yaw,0);
		LastMovementY = rot.Vector() ;
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
	InputMoveX.X = moveVector;
	if(moveVector > 0.005f )
	{
		FRotator rot = FRotator(0,GetControlRotation().Yaw,0);
		LastMovementX = FRotationMatrix(rot).GetScaledAxis(EAxis::Y) ;
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
	InputMoveX.Y = moveVector;
	if(moveVector < -0.005f )
	{
		FRotator rot = FRotator(0,GetControlRotation().Yaw,0);
		LastMovementX = FRotationMatrix(rot).GetScaledAxis(EAxis::Y) ;
		bMoveInputFlipX = true;
	}
	else
	{
		bMoveInputFlipX = false;
	}
}

void ATPCharacterBase::InputEvent_Run(const FInputActionValue& value)
{
	bRunInput = value.Get<bool>();
	bRun = bRunInput;
	if(bAim)
	{
		bRun = false;
	}
	if(RunInputTrigger.IsBound())
	{
		RunInputTrigger.Broadcast(bRun);
	}
}

void ATPCharacterBase::InputEvent_LookAxis2D(const FInputActionValue& value)
{
	auto moveVector = value.Get<FVector2D>();
	LookAxis = moveVector;
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

void ATPCharacterBase::InputEvent_Aim(const FInputActionValue& value)
{
	auto bTrigger = value.Get<bool>();
	bAim = bTrigger;
	bInputAim = bTrigger;
	if(AimInputTrigger.IsBound())
	{
		AimInputTrigger.Broadcast(bAim);
	}
}

void ATPCharacterBase::InputEvent_Fire(const FInputActionValue& value)
{
	auto bTrigger = value.Get<bool>();
	bFire = bTrigger;
	//同时触发瞄准
	if(bInputAim && !bFire)
	{
		bAim = true;
	}
	else
	{
		bAim = bFire;
	}

	if(CurrentWeapon)
	{
		if(CurrentWeapon->CurrentWeaponFiringMode == EWeaponFiringMode::WFM_Automatic)
		{
			if(bFire && FireCount ==-1)
			{
				CurrentFireInterval = CurrentWeapon->FireInterval;
				FireCount = 0;
			}
		}
		else if(CurrentWeapon->CurrentWeaponFiringMode == EWeaponFiringMode::WFM_Single)
		{
			if(bFire && FireCount ==-1)
			{
				FireCount = 0;
				CurrentFireInterval = CurrentWeapon->FireInterval;
			} 
		}
		else if(CurrentWeapon->CurrentWeaponFiringMode == EWeaponFiringMode::WFM_Triple)
		{
			if(bFire && FireCount ==-1)
			{
				FireCount = 0;
				CurrentFireInterval = CurrentWeapon->FireInterval;
			} 
		}
	}
	
	if(FireInputTrigger.IsBound())
	{
		FireInputTrigger.Broadcast(bAim);
	}
}

void ATPCharacterBase::InputEvent_ChangeAimDirection(const FInputActionValue& value)
{
	if(bFlipAnimation)
	{
		ChangeAimDirection(0);
	}
	else
	{
		ChangeAimDirection(1);
	}
}

// Called every frame
void ATPCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
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

#if WITH_EDITOR
	//Hold aim state
	if(bHoldAimState)
	{
		FInputActionValue value = FInputActionValue(1.0f);
		InputEvent_Aim(value);
	}
#endif
	
	if(!bMoveInputX&&!bMoveInputFlipX&&!bMoveInputY&&!bMoveInputFlipY&&CurrentMoveSpeed>10)
	{
		AddMovementInput( GetActorForwardVector() , FMath::Clamp(CurrentMoveSpeed,0,1));
		GetCharacterMovement()->MaxWalkSpeed -= DeltaTime * StopMovementSpeed;
	}
	else if(bMoveInputX || bMoveInputFlipX || bMoveInputY || bMoveInputFlipY)
	//if(bMoveInputX || bMoveInputFlipX || bMoveInputY || bMoveInputFlipY)
	{
		MoveAxis.X = InputMoveX.X + InputMoveX.Y;
		MoveAxis.Y = InputMoveY.X + InputMoveY.Y;
		if(bMoveInputX && bMoveInputFlipX)
			MoveAxis.X = 0;
		if(bMoveInputY && bMoveInputFlipY)
			MoveAxis.Y = 0;
		UpdateMovementSpeed();
		AddMovementInput( LastMovementX , MoveAxis.X);
		AddMovementInput( LastMovementY , MoveAxis.Y);
		if(!FMath::IsNearlyEqual(GetCharacterMovement()->MaxWalkSpeed,TargetMovementSpeed,0.001f))
		{
			if(bRun)
			{
				GetCharacterMovement()->MaxWalkSpeed = FMath::FInterpTo(GetCharacterMovement()->MaxWalkSpeed,TargetMovementSpeed,DeltaTime,BeginRunSpeed);
			}
			else
			{
				GetCharacterMovement()->MaxWalkSpeed = FMath::FInterpTo(GetCharacterMovement()->MaxWalkSpeed,TargetMovementSpeed,DeltaTime,BeginWalkSpeed);
			}
		}
	}
	else if(CurrentMoveSpeed < 5)
	{
		MoveAxis  = FVector2D(0) ;
		TargetMovementSpeed = 0;
		GetCharacterMovement()->MaxWalkSpeed = TargetMovementSpeed;
	}
	
	auto AimOffsetDelteRot = (PlayerCameraComp->GetComponentRotation()-GetActorRotation());
	AimOffsetDelteRot.Normalize();
	AimOffsetDelteRot += AimOffsetRotBias;
	//AimOffsetRot = FMath::RInterpTo(AimOffsetRot,AimOffsetDelteRot,DeltaTime,AimOffsetRotSpeed);
	AimOffsetRot = AimOffsetDelteRot;

	FVector targetViewSocketOffset;
	if(bFlipAnimation)
	{
		targetViewSocketOffset = FVector(10.0f,-60.0f,0.0f);
	}
	else
	{
		targetViewSocketOffset = FVector(10.0f,60.0f,0.0f);
	}
	if(!FMath::IsNearlyEqual(PlayerCameraSpringArmComp->SocketOffset.Y,targetViewSocketOffset.Y,0.1f))
	{
		PlayerCameraSpringArmComp->SocketOffset = FMath::VInterpTo(PlayerCameraSpringArmComp->SocketOffset,targetViewSocketOffset,DeltaTime,25.0f);
	}
	
	//Fire
	if(CurrentWeapon)
	{
		if(CurrentWeaponBulletSpreadSize <= CurrentWeapon->MinBulletSpreadSize)
		{
			CurrentWeaponBulletSpreadSize = CurrentWeapon->MinBulletSpreadSize;
		}
		if(CurrentWeapon->BulletClass && CurrentWeapon->IsWeaponActive())
		{
			if(CurrentWeapon->CurrentWeaponFiringMode == EWeaponFiringMode::WFM_Automatic && bFire)
			{
				CurrentFireInterval+=DeltaTime;
				if(CurrentFireInterval >= CurrentWeapon->FireInterval)
				{
					CurrentFireInterval = 0;
					FireCount = -1;
					FVector Pos;FVector Dir;
					GetBulletSpawnTransform(Pos,Dir);
					for(int i = 0; i < CurrentWeapon->BulletCount ; i++)
					{
						SpawnBullet(Pos,Dir);
					}
					CurrentWeaponBulletSpreadSize+=CurrentWeapon->BulletSpreadAdditiveSize;
				}
			}
			else if(CurrentWeapon->CurrentWeaponFiringMode == EWeaponFiringMode::WFM_Single && FireCount >= 0)
			{
				CurrentFireInterval+=DeltaTime;
				if(CurrentFireInterval >= CurrentWeapon->FireInterval)
				{
					CurrentFireInterval = 0;
					if(FireCount>0)
					{
						FireCount = -1;
					}
					else
					{
						FVector Pos;FVector Dir;
                        GetBulletSpawnTransform(Pos,Dir);
						for(int i = 0; i < CurrentWeapon->BulletCount ; i++)
						{
							SpawnBullet(Pos,Dir);
						}
						CurrentWeaponBulletSpreadSize+=CurrentWeapon->BulletSpreadAdditiveSize;
						FireCount++;
					}
				}
			}
			else if(CurrentWeapon->CurrentWeaponFiringMode == EWeaponFiringMode::WFM_Triple && FireCount >= 0)
			{
				CurrentFireInterval+=DeltaTime;
				if(CurrentFireInterval >= CurrentWeapon->FireInterval)
				{
					CurrentFireInterval = 0;
					if(FireCount >= 3)
					{
						FireCount = -1;
					}
					else
					{
						FVector Pos;FVector Dir;
						GetBulletSpawnTransform(Pos,Dir);
						for(int i = 0; i < CurrentWeapon->BulletCount ; i++)
						{
							SpawnBullet(Pos,Dir);
						}
						FireCount ++;
						CurrentWeaponBulletSpreadSize+=CurrentWeapon->BulletSpreadAdditiveSize;
					}
				}
			}
			if(CurrentWeaponBulletSpreadSize > CurrentWeapon->MinBulletSpreadSize)
			{
				CurrentWeaponBulletSpreadSize -= DeltaTime * CurrentWeapon->BulletSpreadSubtractSpeed;
			}
		}
	}
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
	enhancedInputComp->BindAction(InputLookAxis2D,ETriggerEvent::Completed,this,&ATPCharacterBase::InputEvent_LookAxis2D);
	//Run
	enhancedInputComp->BindAction(InputRun,ETriggerEvent::Triggered,this,&ATPCharacterBase::InputEvent_Run);
	enhancedInputComp->BindAction(InputRun,ETriggerEvent::Completed,this,&ATPCharacterBase::InputEvent_Run);
	//Interactive
	enhancedInputComp->BindAction(InputInteractive,ETriggerEvent::Started,this,&ATPCharacterBase::InputEvent_Interactive);
	//Aim
	enhancedInputComp->BindAction(InputAim,ETriggerEvent::Triggered,this,&ATPCharacterBase::InputEvent_Aim);
	enhancedInputComp->BindAction(InputAim,ETriggerEvent::Completed,this,&ATPCharacterBase::InputEvent_Aim);
	//Fire
	//Aim
	enhancedInputComp->BindAction(InputFire,ETriggerEvent::Started,this,&ATPCharacterBase::InputEvent_Fire);
	enhancedInputComp->BindAction(InputFire,ETriggerEvent::Completed,this,&ATPCharacterBase::InputEvent_Fire);
	//Change aim direction
	enhancedInputComp->BindAction(InputChangeAimDirection,ETriggerEvent::Started,this,&ATPCharacterBase::InputEvent_ChangeAimDirection);
	
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
	PickUpWeapon(nearestActor);
}

void ATPCharacterBase::UpdateMovementSpeed()
{
	if(!bRun)
	{
		if(CurrentWeapon)
		{
			if(bAim)
			{
				TargetMovementSpeed = AimWalkSpeedWithWeapon;
			}
			else
			{
				TargetMovementSpeed = WalkSpeedWithWeapon;
			}
		}
		else
		{
			TargetMovementSpeed = WalkSpeed;
		}
	}
	else
	{
		if(CurrentWeapon)
		{
			TargetMovementSpeed = RunSpeedWithWeapon;
		}
		else
		{
			TargetMovementSpeed = RunSpeed;
		}
	}
}

void ATPCharacterBase::GetBulletSpawnTransform(FVector& Pos,FVector& Dir)
{
	auto trans = CurrentWeapon->GetWeaponComp()->GetSocketTransform("Bullet");
	FHitResult result;
	FCollisionObjectQueryParams objectQueryParams;
	FCollisionQueryParams queryParams = FCollisionQueryParams::DefaultQueryParam;
	queryParams.AddIgnoredActor(this);
	TArray<AActor*> attachActors;
	this->GetAttachedActors(attachActors);
	queryParams.AddIgnoredActors(attachActors);
	queryParams.bTraceComplex = true;//接触复杂碰撞
	objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);
	objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Vehicle);
	
	Dir = PlayerCameraComp->GetForwardVector();
	bool bHit = GetWorld()->LineTraceSingleByObjectType(result,
		trans.GetLocation(),
		PlayerCameraComp->GetComponentLocation() + PlayerCameraComp->GetForwardVector() * 1000000.0f,
		objectQueryParams , queryParams
		);
	if(bHit)
	{
		Dir = (result.Location - trans.GetLocation());
		Dir.Normalize();
	}
	trans.SetRotation(Dir.Rotation().Quaternion());
	
	Pos = trans.GetLocation();
//
// #if ENABLE_DRAW_DEBUG
// 	DrawDebugLineTraceSingle(GetWorld(),
// 		PlayerCameraComp->GetComponentLocation(),
// 		PlayerCameraComp->GetComponentLocation() + PlayerCameraComp->GetForwardVector() * 1000000.0f,
// 		EDrawDebugTrace::Type::ForDuration,
// 		bHit, result, FLinearColor::Red, FLinearColor::Green, 3.0f);
// #endif
	
}

void ATPCharacterBase::SpawnBullet(FVector Pos,FVector Dir)
{
	if(CurrentWeapon && CurrentWeapon->BulletClass)
	{
		float Xscale = FMath::RandRange(-1.0f,1.0f);
		float Yscale = FMath::RandRange(-1.0f,1.0f);
		Dir += CurrentWeaponBulletSpreadSize * PlayerCameraComp->GetRightVector() * Xscale * 0.01f;
		Dir += CurrentWeaponBulletSpreadSize * PlayerCameraComp->GetUpVector() * Yscale * 0.01f;
		auto newRot = Dir.Rotation().Quaternion();
		FTransform SpawnTrans;
		SpawnTrans.SetLocation(Pos);
		SpawnTrans.SetRotation(newRot);
		FActorSpawnParameters spawnParameters;
		spawnParameters.Owner = this;
		spawnParameters.Instigator = GetInstigator();
		spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		spawnParameters.CustomPreSpawnInitalization = [&](AActor*actor)
		{
			ATPBullet* bulletActor = static_cast<ATPBullet*>(actor);
			bulletActor->StartGravityDistance = CurrentWeapon->StartGravityDistance;
			bulletActor->GravityAdditiveSpeed = CurrentWeapon->GravityAdditiveSpeed;
			bulletActor->GetProjectileMovementComponent()->InitialSpeed = CurrentWeapon->BulletSpeedInit * 100.0f;
			bulletActor->BulletSizeChange = CurrentWeapon->BulletSizeChange;
			bulletActor->BulletPower = CurrentWeapon->BulletPower * 0.01f;
		};
		GetWorld()->SpawnActor(CurrentWeapon->BulletClass,&SpawnTrans,spawnParameters);
	}
}

void ATPCharacterBase::PickUpWeapon(AActor* weaponActor)
{
	if(bEquip || bPackUp)
	{
		return;
	}
	if(weaponActor != nullptr)
	{
		auto newWeapon = Cast<ATPWeaponBase>(weaponActor);
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

void ATPCharacterBase::FlipAnimation(bool bLeft)
{
	bFlipAnimation = bLeft;
	if(CurrentWeapon)
	{
		CurrentWeapon->AttachWeaponToCharacter(bLeft);
	}
}

#if WITH_EDITOR
void ATPCharacterBase::HoldAimState()
{
	if(!bHoldAimState)
	{
		bHoldAimState = true;
	}
	else
	{
		bHoldAimState = false;
		FInputActionValue value = FInputActionValue(0.0f);
		InputEvent_Aim(value);
	}
}

void ATPCharacterBase::EnableBulletLineTraceDebug()
{
	if(!ATPBullet::bBulletLineTraceDebug)
	{
		ATPBullet::bBulletLineTraceDebug = true;
	}
	else
	{
		ATPBullet::bBulletLineTraceDebug = false;
	}
}
#endif
