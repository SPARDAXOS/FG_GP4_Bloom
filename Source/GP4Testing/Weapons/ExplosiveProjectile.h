#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosiveProjectile.generated.h"

UCLASS()
class GP4TESTING_API AExplosiveProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AExplosiveProjectile();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
