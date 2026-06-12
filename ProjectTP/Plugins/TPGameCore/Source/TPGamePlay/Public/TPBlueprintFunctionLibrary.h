// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TPBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TPGAMEPLAY_API UTPBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
protected:
	static bool bShowDebugMessage;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static bool IsEditor();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static bool IsShipping();
	
	UFUNCTION(BlueprintCallable, Exec)
	static void ShowDebugMessage(bool bIsShowDebugMessage); 
	
	UFUNCTION(BlueprintCallable,BlueprintPure)
	static bool IsShowDebugMessage() { return bShowDebugMessage; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static UWorld* GetCurentWorld(const UObject* WorldContextObject);
	
};
