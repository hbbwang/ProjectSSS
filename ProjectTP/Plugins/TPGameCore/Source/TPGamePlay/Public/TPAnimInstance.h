// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TPAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UTPAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	void NativeInitializeAnimation() override;
	
	void NativeBeginPlay() override;
	
	void NativeUpdateAnimation(float DeltaSeconds) override;
	
};
