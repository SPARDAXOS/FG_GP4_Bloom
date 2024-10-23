// Fill out your copyright notice in the Description page of Project Settings.


#include "GP4Testing/Pickups/AmmoPickup.h"
#include "GP4Testing/Systems/PrimaryPlayer.h"
#include "GP4Testing/PlayerSystems/WeaponManagementSystem.h"
#include "GP4Testing/Pickups/PickupSpawner.h"
#include <Kismet/GameplayStatics.h>


AAmmoPickup::AAmmoPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	pickupType = PickupType::AMMO;

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

		if (gunRef->AddAmmo(pickupType, value)) {
			if (registeredSpawner)
				registeredSpawner->NotifyPickup(*this);

			Destroy();	

			if (PickupSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, PickupSound, playerRef->GetActorLocation());

			}
		}
	}
}

