// Fill out your copyright notice in the Description page of Project Settings.

#include "TPCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ATPCharacter::ATPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->SetAbsolute(false, true,false);
	SpringArm->SetWorldRotation(FRotator(-60.0f,0,0));
	SpringArm->bInheritYaw = 0;
	SpringArm->bInheritPitch = 0;
	SpringArm->bInheritRoll = 0;
	SpringArm->bDoCollisionTest = 0;
	
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	PlayerCamera->SetupAttachment(SpringArm);
	PlayerCamera->FieldOfView = 60.0f;
	PlayerCamera->bUsePawnControlRotation = 0;
	
	bUseControllerRotationYaw = 1;
	bUseControllerRotationPitch = 0;
	bUseControllerRotationRoll = 0;
	

}

// Called when the game starts or when spawned
void ATPCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

