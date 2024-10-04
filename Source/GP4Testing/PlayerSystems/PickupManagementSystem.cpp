
#include "PickupManagementSystem.h"

bool APickupManagementSystem::RegisterPickup(const PickupType& type, float amount) noexcept
{
	if (type == PickupType::HEALTH)
	{
		amount = 10;
	}

	if (type == PickupType::AMMO)
	{
		amount = 10;
	}

	return true;
}