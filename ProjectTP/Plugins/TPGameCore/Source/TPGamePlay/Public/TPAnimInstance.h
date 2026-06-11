// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TPAnimInstance.generated.h"

/**
 * 
 */

class ATPPlayerController;
class ATPCharacter;

UCLASS()
class UTPAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	void NativeInitializeAnimation() override;
	
	void NativeBeginPlay() override;
	
	void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TPAnim")
	TWeakObjectPtr<ATPCharacter> TPCharacter;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TPAnim")
	TWeakObjectPtr<ATPPlayerController> TPPlayerController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="TPAnim")
	float Speed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="TPAnim")
	FVector2D MovementAxis;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="TPAnim")
	FVector2D LookAxis;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="TPAnim")
	FVector Velocity;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="TPAnim")
	FVector2D VelocityXY;
};
