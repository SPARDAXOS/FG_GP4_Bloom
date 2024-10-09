

#include "WeaponManagementSystem.h"



bool AWeaponManagementSystem::UseCurrentWeapon(bool& input) noexcept {
	if (AcquiredWeapons.Num() > 0)
	{
		UGunComponent* Weapon = AcquiredWeapons.FindRef(EquippedWeapon);

		if (EquippedWeapon == WeaponType::MACHINE_GUN)
		{
			if (input)
			{
				Weapon->StartFire();
			}
			else
			{
				Weapon->StopFire();
			}
		}
		else
		{
			Weapon->StartFire();
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
	UGunComponent* CurrentWeapon = AcquiredWeapons.FindRef(EquippedWeapon);
	CurrentWeapon->SetVisibility(false);
	
	for (auto Weapon = AcquiredWeapons.CreateIterator(); Weapon; ++Weapon)
	{
		if (Weapon->Key == EquippedWeapon)
		{
			auto NextWeapon = ++Weapon;
			NextWeapon->Value->SetVisibility(true);
			EquippedWeapon = NextWeapon->Key;
		}
	}

	return true;
}
bool AWeaponManagementSystem::SwitchPreviousWeapon() noexcept {
	


	return true;
}


bool AWeaponManagementSystem::AcquireWeapon(WeaponType type, UGunComponent* weapon) noexcept {
	if (AcquiredWeapons.Num() > 0)
	{
		// Hide current weapon
		UGunComponent* Weapon = AcquiredWeapons.FindRef(EquippedWeapon);
		Weapon->SetVisibility(false);
	}
	
	AcquiredWeapons.Add(type, weapon);
	EquippedWeapon = type;

	return true;
}


void AWeaponManagementSystem::SetupStartingState() noexcept {

}

UGunComponent* AWeaponManagementSystem::GetCurrentWeapon()
{
	UGunComponent* Weapon = nullptr;
	if (AcquiredWeapons.Num() > 0)
	{
		Weapon = AcquiredWeapons.FindRef(EquippedWeapon);
	}
	return Weapon;
}

TMap<WeaponType, UGunComponent*> AWeaponManagementSystem::GetAcquiredWeapons()
{
	return AcquiredWeapons;
}

float AWeaponManagementSystem::GetMaxAmmo()
{
	UGunComponent* Weapon = nullptr;
	float MaxAmmo = 0;
	if (AcquiredWeapons.Num() > 0)
	{
		Weapon = AcquiredWeapons.FindRef(EquippedWeapon);
		MaxAmmo = Weapon->MaxAmmo;
	}
	return MaxAmmo;
}

float AWeaponManagementSystem::GetAmmo()
{
	UGunComponent* Weapon = nullptr;
	float Ammo = 0;
	if (AcquiredWeapons.Num() > 0)
	{
		Weapon = AcquiredWeapons.FindRef(EquippedWeapon);
		Ammo = Weapon->Ammo;
	}
	return Ammo;
}

void AWeaponManagementSystem::SetAmmo(float newAmmo)
{
	UGunComponent* Weapon = AcquiredWeapons.FindRef(EquippedWeapon);
	if (Weapon != nullptr)
	{
		if (Weapon->Ammo < Weapon->MaxAmmo)
		{
			if ((Weapon->Ammo + newAmmo) > Weapon->MaxAmmo)
			{
				float NoAdd = (Weapon->Ammo + newAmmo) - Weapon->MaxAmmo;
				float Add = newAmmo - NoAdd;
				Weapon->Ammo += Add;
			}
			else
			{
				Weapon->Ammo += newAmmo;
			}
		}
	}
}

float AWeaponManagementSystem::GetMaxMagazine()
{
	UGunComponent* Weapon = nullptr;
	float MaxMag = 0;
	if (AcquiredWeapons.Num() > 0)
	{
		Weapon = AcquiredWeapons.FindRef(EquippedWeapon);
		MaxMag = Weapon->MaxMagazine;
	}
	return MaxMag;
}

float AWeaponManagementSystem::GetLoadedMagazine()
{
	UGunComponent* Weapon = nullptr;
	float Mag = 0;
	if (AcquiredWeapons.Num() > 0)
	{
		Weapon = AcquiredWeapons.FindRef(EquippedWeapon);
		Mag = Weapon->Magazine;
	}
	return Mag;
}
