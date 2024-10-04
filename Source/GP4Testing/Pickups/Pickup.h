// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP4Testing/PlayerSystems/PickupTypes.h"
#include "Pickup.generated.h"

UCLASS()
class GP4TESTING_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();
	FName Tag;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public: 


	UPROPERTY(Editanywhere)
	USceneComponent* PickUpRoot;

	UPROPERTY(Editanywhere)
	class UBoxComponent* PickUpBox;

	UPROPERTY(Editanywhere)
	UStaticMeshComponent* PickUpMesh;

	UFUNCTION()
		void OnPlayerInteraction(UPrimitiveComponent * OverlappedComp, AActor* OtherActor, UPrimitiveComponent* otherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult&SweepResults);

		
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	PickupType pickupType;
};
