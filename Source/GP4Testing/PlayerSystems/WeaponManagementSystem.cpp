

#include "WeaponManagementSystem.h"



bool AWeaponManagementSystem::UseCurrentWeapon() noexcept {


	return true;
}


bool AWeaponManagementSystem::SwitchNextWeapon() noexcept {


	return true;
}
bool AWeaponManagementSystem::SwitchPreviousWeapon() noexcept {


	return true;
}
bool AWeaponManagementSystem::AcquireWeapon(WeaponType type) noexcept {


	return true;
}


void AWeaponManagementSystem::SetupStartingState() noexcept {

}

void AWeaponManagementSystem::SetHasWeapon(bool hasWeapon)
{
	bHasWeapon = hasWeapon;
}

bool AWeaponManagementSystem::GetHasWeapon()
{
	return bHasWeapon;
}
