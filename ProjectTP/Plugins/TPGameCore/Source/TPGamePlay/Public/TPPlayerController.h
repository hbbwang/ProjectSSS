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
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly,Category="TPInput")
	TObjectPtr<UInputMappingContext> InputMappingContext0;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly,Category="TPInput")
	TObjectPtr<UInputAction> ForwardAndBackward;
	UFUNCTION()
	void ActionFunc_ForwardAndBackward(const FInputActionValue& Value);
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly,Category="TPInput")
	TObjectPtr<UInputAction> RightwardAndLeftward;
	UFUNCTION()
	void ActionFunc_RightwardAndLeftward(const FInputActionValue& Value);
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly,Category="TPInput")
	TObjectPtr<UInputAction> Look;
	UFUNCTION()
	void ActionFunc_Look(const FInputActionValue& Value);

	//运行时修改按键
	UFUNCTION(Blueprintable)
	void RebindActionKey(UInputMappingContext* MappingContext, UInputAction* Action, FKey NewKey);
	
	//刷新映射(修改了按键之后需要调用)
	UFUNCTION(Blueprintable)
	void RequestInputRebind();
	
};
