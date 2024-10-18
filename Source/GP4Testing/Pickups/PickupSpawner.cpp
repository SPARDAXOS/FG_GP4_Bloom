
#include "PickupSpawner.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GP4Testing/Pickups/Pickup.h"
#include "GP4Testing/Utility/Debugging.h"

#include <functional>


APickupSpawner::APickupSpawner() {
    PrimaryActorTick.bCanEverTick = true;

    root = CreateDefaultSubobject<USceneComponent>("Root");
    SetRootComponent(root);

    spawnPoint = CreateDefaultSubobject<USceneComponent>("SpawnPoint");
    spawnPoint->SetupAttachment(root);

    mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    mesh->SetupAttachment(root);
}


void APickupSpawner::BeginPlay() {
    Super::BeginPlay();

    respawnTimer.SetLengthRef(&respawnDuration);
    respawnTimer.SetOnCompletedCallback(std::bind(&APickupSpawner::Respawn, this));
    if (spawnAtStart || (!spawnAtStart && respawnDuration <= 0.0f))
        Respawn();
}
void APickupSpawner::Tick(float deltaTime) {
    Super::Tick(deltaTime);
    if (pickupClass && !pickupSpawned)
        respawnTimer.Update(deltaTime);
}

void APickupSpawner::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    Super::EndPlay(EndPlayReason); 

    if (pickupRef) {
        pickupRef->Destroy();  
        pickupRef = nullptr;   
        pickupSpawned = false; 
    }
}


void APickupSpawner::NotifyPickup(APickup& outer) {
    if (!pickupSpawned || !pickupRef)
        return;

    if (outer.GetUniqueID() == pickupRef->GetUniqueID()) {
        pickupRef = nullptr;
        pickupSpawned = false;
        if (respawnDuration <= 0.0f)
            Respawn();
    }
}
void APickupSpawner::Respawn() {
    if (!pickupClass || pickupSpawned)
        return;

    pickupRef = GetWorld()->SpawnActor<APickup>(pickupClass);
    pickupRef->RegisterPickupSpawner(*this);
    pickupRef->SetActorLocation(spawnPoint->GetComponentLocation());
    pickupSpawned = true;
}






