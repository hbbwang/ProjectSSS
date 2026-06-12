// Fill out your copyright notice in the Description page of Project Settings.

#include "TPPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TPCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

ATPPlayerController::ATPPlayerController()
{
}

void ATPPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ATPPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	float MouseX = 0,MouseY = 0;
	if (GetMousePosition(MouseX,MouseY))
	{
		FVector MouseWP,MouseWD;
		if (UGameplayStatics::DeprojectScreenToWorld(this, FVector2D(MouseX,MouseY),MouseWP,MouseWD))
		{
			s
		}
	}
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
			EnhancedInputComponent->BindAction(Forward,ETriggerEvent::Triggered, this, &ATPPlayerController::ActionFunc_Forward);
			EnhancedInputComponent->BindAction(Rightward,ETriggerEvent::Triggered, this, &ATPPlayerController::ActionFunc_Rightward);
			EnhancedInputComponent->BindAction(Forward,ETriggerEvent::Completed, this, &ATPPlayerController::ActionFunc_Forward);
			EnhancedInputComponent->BindAction(Rightward,ETriggerEvent::Completed, this, &ATPPlayerController::ActionFunc_Rightward);
			EnhancedInputComponent->BindAction(Look,ETriggerEvent::Triggered, this, &ATPPlayerController::ActionFunc_Look);
		}
	}
}

void ATPPlayerController::ActionFunc_Forward(const FInputActionValue& Value)
{
	const float Axis = Value.Get<float>();
	if (ATPCharacter* TPCharacter = Cast<ATPCharacter>(GetPawn()))
	{
		ForwardAxis = Axis;
		FVector MoveVector = TPCharacter->GetPlayerCamera()->GetForwardVector();
		MoveVector = FVector(FVector2D(MoveVector),0);
		MoveVector.Normalize();
		TPCharacter->AddMovementInput(MoveVector, Axis);
	}
	else
	{
		ForwardAxis = 0;
	}
}

void ATPPlayerController::ActionFunc_Rightward(const FInputActionValue& Value)
{
	const float Axis = Value.Get<float>();
	if (ATPCharacter* TPCharacter = Cast<ATPCharacter>(GetPawn()))
	{
		RightwardAxis = Axis;
		FVector MoveVector = TPCharacter->GetPlayerCamera()->GetRightVector();
		MoveVector = FVector(FVector2D(MoveVector),0);
		MoveVector.Normalize();
		TPCharacter->AddMovementInput(MoveVector, Axis);
	}
	else
	{
		RightwardAxis = 0;
	}
}

void ATPPlayerController::ActionFunc_Look(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	LookAxis = Axis;
	// AddYawInput(Axis.X);
	// AddPitchInput(Axis.Y);
}

void ATPPlayerController::RebindActionKey(UInputMappingContext* MappingContext, UInputAction* Action, FKey NewKey)
{
	if (MappingContext && Action && NewKey.IsValid())
	{
		auto & Mappings = MappingContext->GetMappings();
		TArray<TObjectPtr<UInputModifier>> OldModifiers;
		TArray<TObjectPtr<UInputTrigger>> OldTriggers;
		//解除旧按键绑定
		for (auto& m:Mappings)
		{
			if (m.Action == Action)
			{
				MappingContext->UnmapKey(Action, m.Key);
				OldModifiers = m.Modifiers;
				OldTriggers = m.Triggers;
			}
		}
		//设置新的
		FEnhancedActionKeyMapping& NewMapping = MappingContext->MapKey(Action,NewKey);
		NewMapping.Modifiers = OldModifiers;
		NewMapping.Triggers = OldTriggers;
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

