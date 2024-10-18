
#include "WeaponSpawner.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GP4Testing/Weapons/GunComponent.h"
#include "GP4Testing/Utility/Debugging.h"

#include <functional>


AWeaponSpawner::AWeaponSpawner() {
    PrimaryActorTick.bCanEverTick = true;

    root = CreateDefaultSubobject<USceneComponent>("Root");
    SetRootComponent(root);

    spawnPoint = CreateDefaultSubobject<USceneComponent>("SpawnPoint");
    spawnPoint->SetupAttachment(root);

    mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    mesh->SetupAttachment(root);
}


void AWeaponSpawner::BeginPlay() {
    Super::BeginPlay();

    respawnTimer.SetLengthRef(&respawnDuration);
    respawnTimer.SetOnCompletedCallback(std::bind(&AWeaponSpawner::Respawn, this));
    if (spawnAtStart || (!spawnAtStart && respawnDuration <= 0.0f))
        Respawn();
}
void AWeaponSpawner::Tick(float deltaTime) {
    Super::Tick(deltaTime);
    if (weaponPickupClass && !pickupSpawned)
        respawnTimer.Update(deltaTime);
}

void AWeaponSpawner::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    Super::EndPlay(EndPlayReason); 

    if (pickupRef) {
        pickupRef->Destroy();  
        pickupRef = nullptr;   
        pickupSpawned = false; 
    }
}


void AWeaponSpawner::NotifyPickup(AGunComponent& outer) {
    if (!pickupSpawned || !pickupRef)
        return;

    if (outer.GetUniqueID() == pickupRef->GetUniqueID()) {
        pickupRef = nullptr;
        pickupSpawned = false;
        if (respawnDuration <= 0.0f)
            Respawn();
    }
}
void AWeaponSpawner::Respawn() {
    if (!weaponPickupClass || pickupSpawned)
        return;

    pickupRef = GetWorld()->SpawnActor<AGunComponent>(weaponPickupClass);
    //pickupRef->RegisterPickupSpawner(*this);
    pickupRef->SetActorLocation(spawnPoint->GetComponentLocation());
    pickupSpawned = true;
}






