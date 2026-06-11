// Fill out your copyright notice in the Description page of Project Settings.


#include "TPAnimInstance.h"

#include "TPCharacter.h"
#include "TPPlayerController.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void UTPAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UTPAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UTPAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!IsValid(TryGetPawnOwner()))
		return;
	
	if (!TPCharacter.IsValid())
	{
		TPCharacter = Cast<ATPCharacter>(TryGetPawnOwner());
	}
	
	if (!TPPlayerController.IsValid())
	{
		if (TryGetPawnOwner()->GetController() && TryGetPawnOwner()->GetController()->IsLocalController())
		{
			TPPlayerController = Cast<ATPPlayerController>(TryGetPawnOwner()->GetController());
		}
	}
	
	if (!TPCharacter.IsValid() || !TPPlayerController.IsValid())
		return;
	
	Velocity = TPCharacter->GetMovementComponent()->Velocity;
	
	VelocityXY = FVector2D(Velocity.X, Velocity.Y);
	
	Speed = Velocity.Length();

	MovementAxis = FVector2D(TPPlayerController->GetRightwardAxis(), TPPlayerController->GetForwardAxis());
	
	LookAxis = TPPlayerController->GetLookAxis();
	
	
}
