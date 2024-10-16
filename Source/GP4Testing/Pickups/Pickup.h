// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP4Testing/Components/HealthComponent.h"
#include "GP4Testing/PlayerSystems/PickupTypes.h"
#include "Pickup.generated.h"


class UNiagaraComponent;
class APickupSpawner;

UCLASS(Abstract)
class GP4TESTING_API APickup : public AActor {
	GENERATED_BODY()
	
public:	
	APickup();

public:
	inline void RegisterPickupSpawner(APickupSpawner& spawner) noexcept { registeredSpawner = &spawner; }
	
protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

public: 

	UPROPERTY(Editanywhere)
	USceneComponent* PickUpRoot;

	UPROPERTY(Editanywhere)
	class UBoxComponent* PickUpBox;

	UPROPERTY(Editanywhere)
	UStaticMeshComponent* PickUpMesh;

	UPROPERTY(Editanywhere)
	UNiagaraComponent* vfx;


	UFUNCTION()
	void OnPlayerInteraction(UPrimitiveComponent * OverlappedComp, AActor* OtherActor, UPrimitiveComponent* otherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult&SweepResults);

protected:
	virtual void OnPickup(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* otherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResults);
		
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	FRotator RotationRate;

	UPROPERTY(EditDefaultsOnly)
	PickupType pickupType;

protected:
	FName Tag;
	APickupSpawner* registeredSpawner = nullptr;
};
