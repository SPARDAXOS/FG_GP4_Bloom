// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP4Testing/PlayerSystems/PickupManagementSystem.h"
#include "GP4Testing/Weapons/GunComponent.h"
#include "Pickup.h"
#include "GameFramework/Actor.h"
#include "AmmoPickup.generated.h"


UCLASS(Abstract)
class GP4TESTING_API AAmmoPickup : public APickup {
	GENERATED_BODY()
	
public:	

	AAmmoPickup();


public:
	UPROPERTY(EditDefaultsOnly, Category = "AmmoPickup")
	int value = 1;

protected:
	virtual void OnPickup(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* otherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResults) override;
};
