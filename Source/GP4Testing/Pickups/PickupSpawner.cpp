
#include "PickupSpawner.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GP4Testing/Pickups/Pickup.h"


APickupSpawner::APickupSpawner()
{
    PrimaryActorTick.bCanEverTick = false;

    root = CreateDefaultSubobject<USceneComponent>("Root");
    SetRootComponent(root);

    spawnPoint = CreateDefaultSubobject<USceneComponent>("SpawnPoint");
    spawnPoint->SetupAttachment(root);

    mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    mesh->SetupAttachment(root);
}

void APickupSpawner::BeginPlay()
{
    Super::BeginPlay();
    
    Respawn();
}

void APickupSpawner::Respawn()
{
    if (!pickupClass)
        return;

    pickupRef = GetWorld()->SpawnActor<APickup>(pickupClass);
    pickupRef->SetActorLocation(spawnPoint->GetComponentLocation());

}






