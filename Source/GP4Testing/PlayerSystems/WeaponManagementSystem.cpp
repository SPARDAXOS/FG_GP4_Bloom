

#include "WeaponManagementSystem.h"



bool AWeaponManagementSystem::UseCurrentWeapon(bool& input) noexcept {



	return true;
}

bool AWeaponManagementSystem::ReloadWeapon() noexcept
{

	return false;
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
