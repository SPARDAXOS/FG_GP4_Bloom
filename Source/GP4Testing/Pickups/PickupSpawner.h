
#pragma once

#include "CoreMinimal.h"
#include "PickupSpawner.generated.h"

class APickup;


UCLASS()
class APickupSpawner : public AActor
{
    GENERATED_BODY()

public:
    APickupSpawner();

protected:
    virtual void BeginPlay() override;

private:
    void Respawn();

private:
    UPROPERTY(EditDefaultsOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<APickup> pickupClass; 

    UPROPERTY(EditDefaultsOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
    float respawnDuration;

private:
    UPROPERTY(VisibleAnywhere)
    USceneComponent* root;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* spawnPoint;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* mesh;

private:
    TObjectPtr<APickup> pickupRef;


};
