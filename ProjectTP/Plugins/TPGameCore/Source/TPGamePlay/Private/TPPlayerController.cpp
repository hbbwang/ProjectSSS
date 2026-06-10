// Fill out your copyright notice in the Description page of Project Settings.

#include "TPPlayerController.h"
#include "EnhancedInputSubsystems.h"

void ATPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (auto EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		EnhancedInputLocalPlayerSubsystem->AddMappingContext(InputMappingContext0, 0);
	}
	
}
