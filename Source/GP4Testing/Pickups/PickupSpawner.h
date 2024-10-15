
#pragma once

#include "CoreMinimal.h"
#include "GP4Testing/Utility/Timer.h"
#include "PickupSpawner.generated.h"

class APickup;


UCLASS(Abstract)
class APickupSpawner : public AActor {

    GENERATED_BODY()

public:
    APickupSpawner();

public:
    void NotifyPickup(APickup& outer);

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float deltaTime) override;

private:
    void Respawn();

private:
    UPROPERTY(EditDefaultsOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<APickup> pickupClass; 

    UPROPERTY(EditDefaultsOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
    float respawnDuration = 1.0f;

    UPROPERTY(VisibleAnywhere, Category = "Debugging", meta = (AllowPrivateAccess = "true"))
    bool pickupSpawned = false;

private:
    UPROPERTY(VisibleAnywhere)
    USceneComponent* root;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* spawnPoint;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* mesh;

private:
    TObjectPtr<APickup> pickupRef;
    Timer respawnTimer;

};
