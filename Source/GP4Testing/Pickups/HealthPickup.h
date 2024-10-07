// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.h"
#include "GP4Testing/PlayerSystems/PickupManagementSystem.h"
#include "HealthPickup.generated.h"

UCLASS()
class GP4TESTING_API AHealthPickup : public APickup
{
	GENERATED_BODY()
	
public:	
	virtual void Tick(float deltaTime) override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "HealthPickup")
	float value = 1.0f;

public:
	AHealthPickup();

};
