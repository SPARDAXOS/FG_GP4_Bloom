

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ImpactVFX.generated.h"

class UNiagaraComponent;

UCLASS()
class GP4TESTING_API AImpactVFX : public AActor
{
	GENERATED_BODY()
	
public:	
	AImpactVFX();

	void Activate();


public:
	UPROPERTY(EditAnywhere)
	USceneComponent* root;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* VFX;
};
