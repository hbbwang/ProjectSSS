// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "TPPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TPGAMEPLAY_API ATPPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	void SetupInputComponent() override;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly,Category="TPGamePlay")
	TObjectPtr<UInputMappingContext> InputMappingContext0;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly,Category="TPGamePlay")
	TObjectPtr<UInputAction> Forward;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly,Category="TPGamePlay")
	TObjectPtr<UInputAction> Backward;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly,Category="TPGamePlay")
	TObjectPtr<UInputAction> Left;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly,Category="TPGamePlay")
	TObjectPtr<UInputAction> Right;
	
		
};
