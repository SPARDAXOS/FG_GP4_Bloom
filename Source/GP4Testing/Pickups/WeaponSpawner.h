
#pragma once

#include "CoreMinimal.h"
#include "GP4Testing/Utility/Timer.h"
#include "WeaponSpawner.generated.h"


class AGunComponent;


UCLASS(Abstract)
class AWeaponSpawner : public AActor {

    GENERATED_BODY()

public:
    AWeaponSpawner();

public:
    void NotifyPickup(AGunComponent& outer);

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float deltaTime) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    void Respawn();

private:
    UPROPERTY(EditDefaultsOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<AGunComponent> weaponPickupClass;

    UPROPERTY(EditAnywhere, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
    float respawnDuration = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
    bool spawnAtStart = true;

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
    TObjectPtr<AGunComponent> pickupRef;
    Timer respawnTimer;
};
