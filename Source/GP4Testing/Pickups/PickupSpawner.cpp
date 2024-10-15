
#include "PickupSpawner.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GP4Testing/Pickups/Pickup.h"

#include <functional>


APickupSpawner::APickupSpawner()
{
    PrimaryActorTick.bCanEverTick = true;

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
    respawnTimer.SetLengthRef(&respawnDuration);
    respawnTimer.SetOnCompletedCallback(std::bind(&APickupSpawner::Respawn, this));
    Respawn();
}

void APickupSpawner::Tick(float deltaTime)
{
    Super::Tick(deltaTime);
    if (pickupClass && !pickupRef)
        respawnTimer.Update(deltaTime);

}

void APickupSpawner::Respawn()
{
    if (!pickupClass || pickupRef)
        return;

    pickupRef = GetWorld()->SpawnActor<APickup>(pickupClass);
    pickupRef->SetActorLocation(spawnPoint->GetComponentLocation());

}






