
#include "PickupManagementSystem.h"
#include "GP4Testing/Systems/PrimaryPlayer.h"
#include "GP4Testing/PlayerSystems/PlayerMovementSystem.h" //For movement related stuff
#include "GP4Testing/PlayerSystems/PlayerHealthSystem.h" //For health related stuff
#include "GP4Testing/PlayerSystems/WeaponManagementSystem.h" //For picking up weapons


#include "GP4Testing/Pickups/HealthPickup.h"
#include "GP4Testing/Pickups/Pickup.h"

bool APickupManagementSystem::RegisterPickup(APickup& pickup) noexcept
{
	

	if (pickup.pickupType == PickupType::HEALTH)
	{
		AHealthPickup* targetPickup = Cast<AHealthPickup>(&pickup);
		if (!targetPickup)
			return false;

		APlayerHealthSystem* healthSystem = &primaryPlayerRef->GetPlayerHealthSystem();
		if (!healthSystem)
			return false;

		//return 	healthSystem->AddHealth(targetPickup->value);
	}

	if (pickup.pickupType == PickupType::AMMO)
	{
		
	}

	return true;
}