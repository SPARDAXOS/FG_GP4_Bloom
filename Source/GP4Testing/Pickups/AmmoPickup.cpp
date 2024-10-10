// Fill out your copyright notice in the Description page of Project Settings.


#include "GP4Testing/Pickups/AmmoPickup.h"
#include "GP4Testing/Systems/PrimaryPlayer.h"
#include "GP4Testing/PlayerSystems/WeaponManagementSystem.h"

// Sets default values
AAmmoPickup::AAmmoPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	pickupType = PickupType::AMMO;

}

// Called when the game starts or when spawned
void AAmmoPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAmmoPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAmmoPickup::OnPickup(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* otherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResults)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		APrimaryPlayer* playerRef = Cast<APrimaryPlayer>(OtherActor);
		if (!playerRef)
			return;

		AWeaponManagementSystem* gunRef = &playerRef->GetWeaponManagementSystem();
		if (!gunRef)
			return;
		

		/*gunRef->SetAmmo(10);*/

		Destroy();
		UE_LOG(LogTemp, Warning, TEXT("its AMMO"));
	}
}

