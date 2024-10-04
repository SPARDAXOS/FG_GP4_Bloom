#include "ExplosiveProjectile.h"

AExplosiveProjectile::AExplosiveProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AExplosiveProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AExplosiveProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

