// Fill out your copyright notice in the Description page of Project Settings.


#include "TPBlueprintFunctionLibrary.h"

bool UTPBlueprintFunctionLibrary::bShowDebugMessage = false;

bool UTPBlueprintFunctionLibrary::IsEditor()
{
	#if WITH_EDITOR
	return true;
	#else
	return false;
	#endif
}

bool UTPBlueprintFunctionLibrary::IsShipping()
{
	#if UE_BUILD_SHIPPING
		return true;
	#else
		return false;
	#endif
}

void UTPBlueprintFunctionLibrary::ShowDebugMessage(bool bIsShowDebugMessage)
{
	if ( !IsShipping())
	{
		bShowDebugMessage = bIsShowDebugMessage;
	}
	else
	{
		bShowDebugMessage = false;
	}
}

UWorld* UTPBlueprintFunctionLibrary::GetCurentWorld(const UObject* WorldContextObject)
{
	if (GEngine && WorldContextObject)
	{
		UWorld* ContextWorld = GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::ReturnNull);
		if (ContextWorld)
			return ContextWorld;
	}
	
	if (!GEngine)
		return nullptr;
	
	#if WITH_EDITOR
	if (GEditor && GEditor->GetEditorWorldContext().World())
	{
		return GEditor->GetEditorWorldContext().World();
	}
	#endif
	
	return nullptr;
}
