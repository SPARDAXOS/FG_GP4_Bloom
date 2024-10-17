// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.h"
#include "GP4Testing/Components/HealthComponent.h"
#include "GP4Testing/PlayerSystems/PlayerHealthSystem.h"
#include "GP4Testing/PlayerSystems/PickupManagementSystem.h"
#include "HealthPickup.generated.h"


UCLASS(Abstract)
class GP4TESTING_API AHealthPickup : public APickup {

	GENERATED_BODY()

public:
	AHealthPickup();
	
public:	
	virtual void Tick(float deltaTime) override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "HealthPickup")
	float value = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* PickupSound;


protected:
	virtual void OnPickup(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* otherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResults) override;
};
