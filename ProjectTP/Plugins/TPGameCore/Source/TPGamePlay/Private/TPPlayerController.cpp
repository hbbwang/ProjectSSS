// Fill out your copyright notice in the Description page of Project Settings.

#include "TPPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TPCharacter.h"
#include "Camera/CameraComponent.h"

ATPPlayerController::ATPPlayerController()
{
}

void ATPPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ATPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (IsValid(InputMappingContext0))
	{
		if (auto EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			EnhancedInputLocalPlayerSubsystem->AddMappingContext(InputMappingContext0, 0);
		}
		if (auto EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
		{
			EnhancedInputComponent->BindAction(ForwardAndBackward,ETriggerEvent::Triggered, this, &ATPPlayerController::ActionFunc_ForwardAndBackward);
			EnhancedInputComponent->BindAction(RightwardAndLeftward,ETriggerEvent::Triggered, this, &ATPPlayerController::ActionFunc_RightwardAndLeftward);
			EnhancedInputComponent->BindAction(Look,ETriggerEvent::Triggered, this, &ATPPlayerController::ActionFunc_Look);
		}
	}
}

void ATPPlayerController::ActionFunc_ForwardAndBackward(const FInputActionValue& Value)
{
	const float Axis = Value.Get<float>();
	if (ATPCharacter* TPCharacter = Cast<ATPCharacter>(GetPawn()))
	{
		FVector MoveVector = TPCharacter->GetPlayerCamera()->GetForwardVector();
		MoveVector = FVector(FVector2D(MoveVector),0);
		MoveVector.Normalize();
		TPCharacter->AddMovementInput(MoveVector, Axis);
	}
}

void ATPPlayerController::ActionFunc_RightwardAndLeftward(const FInputActionValue& Value)
{
	const float Axis = Value.Get<float>();
	if (ATPCharacter* TPCharacter = Cast<ATPCharacter>(GetPawn()))
	{
		FVector MoveVector = TPCharacter->GetPlayerCamera()->GetRightVector();
		MoveVector = FVector(FVector2D(MoveVector),0);
		MoveVector.Normalize();
		TPCharacter->AddMovementInput(MoveVector, Axis);
	}
}

void ATPPlayerController::ActionFunc_Look(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	AddYawInput(Axis.X);
	AddPitchInput(Axis.Y);
}

void ATPPlayerController::RebindActionKey(UInputMappingContext* MappingContext, UInputAction* Action, FKey NewKey)
{
	if (MappingContext && Action && NewKey.IsValid())
	{
		auto & Mappings = MappingContext->GetMappings();
		//解除旧按键绑定
		for (auto& m:Mappings)
		{
			if (m.Action == Action)
			{
				MappingContext->UnmapKey(Action, m.Key);
			}
		}
		//设置新的
		MappingContext->MapKey(Action,NewKey);
		MappingContext->Modify();
	}
}

void ATPPlayerController::RequestInputRebind()
{
	if (auto EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		EnhancedInputLocalPlayerSubsystem->RequestRebuildControlMappings();
	}
}

