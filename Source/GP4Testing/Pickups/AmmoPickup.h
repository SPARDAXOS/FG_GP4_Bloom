// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP4Testing/PlayerSystems/PickupManagementSystem.h"
#include "GP4Testing/Weapons/GunComponent.h"
#include "Pickup.h"
#include "GameFramework/Actor.h"
#include "AmmoPickup.generated.h"


UCLASS()
class GP4TESTING_API AAmmoPickup : public APickup
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmoPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "AmmoPickup")
	float value = 1.0f;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnPickup(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* otherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResults);
};
