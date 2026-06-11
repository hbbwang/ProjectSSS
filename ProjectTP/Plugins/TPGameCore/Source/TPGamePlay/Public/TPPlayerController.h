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
public:
	ATPPlayerController();
	
protected:
	void BeginPlay() override;
	
	void SetupInputComponent() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="TPInput")
	TObjectPtr<UInputMappingContext> InputMappingContext0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="TPInput")
	TObjectPtr<UInputAction> Forward;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly,Category="TPInput")
	float ForwardAxis;
	UFUNCTION()
	void ActionFunc_Forward(const FInputActionValue& Value);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="TPInput")
	TObjectPtr<UInputAction> Rightward;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly,Category="TPInput")
	float RightwardAxis;
	UFUNCTION()
	void ActionFunc_Rightward(const FInputActionValue& Value);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="TPInput")
	TObjectPtr<UInputAction> Look;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly,Category="TPInput")
	FVector2D LookAxis;
	UFUNCTION()
	void ActionFunc_Look(const FInputActionValue& Value);

	//运行时修改按键
	UFUNCTION(Blueprintable)
	void RebindActionKey(UInputMappingContext* MappingContext, UInputAction* Action, FKey NewKey);
	
	//刷新映射(修改了按键之后需要调用)
	UFUNCTION(Blueprintable)
	void RequestInputRebind();
	
public:
	UFUNCTION(Blueprintable)
	FVector2D GetLookAxis()const {return LookAxis;}
	
	UFUNCTION(Blueprintable)
	float GetForwardAxis()const {return ForwardAxis;}
	
	UFUNCTION(Blueprintable)
	float GetRightwardAxis()const {return RightwardAxis;}
	
};
