
#pragma once

#include "CoreMinimal.h"
#include "Game/TPCharacterBase.h"
#include "GameFramework/Actor.h"
#include "Game/Interactive/TPInteractive.h"
#include "TPInteractiveActor.generated.h"

UCLASS()
class PROJECTSSS_API ATPInteractiveActor : public AActor,public ITPInteractive
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPInteractiveActor();

	inline virtual FVector GetInteractiveLocation()
	{
		return GetActorLocation();
	}

	inline virtual void SetInteractiveOwner(class ATPCharacterBase* owner)
	{
		InteractiveOwner = owner;
	}

	inline virtual ATPCharacterBase* GetInteractiveOwner()const
	{
		return InteractiveOwner;
	}

protected:

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Weapon",meta=(AllowPrivateAccess="true"))
	ATPCharacterBase* InteractiveOwner;
	
};
