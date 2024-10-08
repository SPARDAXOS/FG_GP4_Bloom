

#include "WeaponManagementSystem.h"



bool AWeaponManagementSystem::UseCurrentWeapon(bool& input) noexcept {
	if (AcquiredWeapons.Num() > 0)
	{
		UGunComponent* Weapon = AcquiredWeapons.FindRef(EquippedWeapon);

		if (input)
		{
			Weapon->StartFire();
		}
		else
		{
			Weapon->StopFire();
		}
	}

	return true;
}

bool AWeaponManagementSystem::ReloadWeapon() noexcept
{
	if (AcquiredWeapons.Num() > 0)
	{
		UGunComponent* Weapon = AcquiredWeapons.FindRef(EquippedWeapon);
		Weapon->Reload();
	}
	
	return true;
}


bool AWeaponManagementSystem::SwitchNextWeapon() noexcept {


	return true;
}
bool AWeaponManagementSystem::SwitchPreviousWeapon() noexcept {


	return true;
}
bool AWeaponManagementSystem::AcquireWeapon(WeaponType type, UGunComponent* weapon) noexcept {
	/*if (AcquiredWeapons.Num() > 0)
	{
		
	}*/
	
	AcquiredWeapons.Add(type, weapon);
	EquippedWeapon = type;

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
