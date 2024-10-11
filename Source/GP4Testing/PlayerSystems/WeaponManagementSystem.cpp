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
	if (AcquiredWeapons.Num() > 1)
	{
		UGunComponent* Weapon = AcquiredWeapons.FindRef(EquippedWeapon);
		Weapon->StopFire();
		Weapon->SetVisibility(false);

		TArray<WeaponType> weapons;
		for (auto it = AcquiredWeapons.CreateConstIterator(); it; ++it) {
			weapons.Add(it->Key);
		}

		int32 CurrentIndex = weapons.Find(EquippedWeapon);

		if (CurrentIndex != weapons.Num() - 1)
		{
			int32 NextIndex = CurrentIndex + 1;
			EquippedWeapon = weapons[NextIndex];
			UGunComponent* NextWeapon = AcquiredWeapons.FindRef(EquippedWeapon);
			NextWeapon->SetVisibility(true);
		}
		else if (CurrentIndex == weapons.Num() - 1)
		{
			EquippedWeapon = weapons[0];
			UGunComponent* NextWeapon = AcquiredWeapons.FindRef(EquippedWeapon);
			NextWeapon->SetVisibility(true);
		}
	}

	return true;
}
bool AWeaponManagementSystem::SwitchPreviousWeapon() noexcept {
	if (AcquiredWeapons.Num() > 1)
	{
		UGunComponent* Weapon = AcquiredWeapons.FindRef(EquippedWeapon);
		Weapon->StopFire();
		Weapon->SetVisibility(false);

		TArray<WeaponType> weapons;
		for (auto it = AcquiredWeapons.CreateConstIterator(); it; ++it) {
			weapons.Add(it->Key);
		}

		int32 CurrentIndex = weapons.Find(EquippedWeapon);

		if (CurrentIndex != 0)
		{
			int32 NextIndex = CurrentIndex - 1;
			EquippedWeapon = weapons[NextIndex];
			UGunComponent* NextWeapon = AcquiredWeapons.FindRef(EquippedWeapon);
			NextWeapon->SetVisibility(true);
		}
		else if (CurrentIndex == 0)
		{
			EquippedWeapon = weapons[weapons.Num() - 1];
			UGunComponent* NextWeapon = AcquiredWeapons.FindRef(EquippedWeapon);
			NextWeapon->SetVisibility(true);
		}
	}

	return true;
}

bool AWeaponManagementSystem::WeaponSlot1() noexcept
{
	if (AcquiredWeapons.Num() > 0)
	{
		UGunComponent* Weapon = AcquiredWeapons.FindRef(EquippedWeapon);
		Weapon->StopFire();
		Weapon->SetVisibility(false);

		TArray<WeaponType> weapons;
		for (auto it = AcquiredWeapons.CreateConstIterator(); it; ++it) {
			weapons.Add(it->Key);
		}

		UGunComponent* NextWeapon = AcquiredWeapons.FindRef(weapons[0]);
		NextWeapon->SetVisibility(true);
		EquippedWeapon = weapons[0];
	}
	
	return false;
}

bool AWeaponManagementSystem::WeaponSlot2() noexcept
{
	if (AcquiredWeapons.Num() > 1)
	{
		UGunComponent* Weapon = AcquiredWeapons.FindRef(EquippedWeapon);
		Weapon->StopFire();
		Weapon->SetVisibility(false);

		TArray<WeaponType> weapons;
		for (auto it = AcquiredWeapons.CreateConstIterator(); it; ++it) {
			weapons.Add(it->Key);
		}

		UGunComponent* NextWeapon = AcquiredWeapons.FindRef(weapons[1]);
		NextWeapon->SetVisibility(true);
		EquippedWeapon = weapons[1];
	}
	
	return false;
}

bool AWeaponManagementSystem::WeaponSlot3() noexcept
{
	if (AcquiredWeapons.Num() > 2)
	{
		UGunComponent* Weapon = AcquiredWeapons.FindRef(EquippedWeapon);
		Weapon->StopFire();
		Weapon->SetVisibility(false);

		TArray<WeaponType> weapons;
		for (auto it = AcquiredWeapons.CreateConstIterator(); it; ++it) {
			weapons.Add(it->Key);
		}

		UGunComponent* NextWeapon = AcquiredWeapons.FindRef(weapons[2]);
		NextWeapon->SetVisibility(true);
		EquippedWeapon = weapons[2];
	}
	
	return false;
}


bool AWeaponManagementSystem::AcquireWeapon(WeaponType type, UGunComponent* weapon) noexcept {
	if (AcquiredWeapons.Num() >= 1)
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

int AWeaponManagementSystem::GetMaxAmmo()
{
	UGunComponent* Weapon = nullptr;
	int MaxAmmo = 0;
	if (AcquiredWeapons.Num() > 0)
	{
		Weapon = AcquiredWeapons.FindRef(EquippedWeapon);
		MaxAmmo = Weapon->MaxAmmo;
	}
	return MaxAmmo;
}

int AWeaponManagementSystem::GetAmmo()
{
	UGunComponent* Weapon = nullptr;
	int Ammo = 0;
	if (AcquiredWeapons.Num() > 0)
	{
		Weapon = AcquiredWeapons.FindRef(EquippedWeapon);
		Ammo = Weapon->Ammo;
	}
	return Ammo;
}

bool AWeaponManagementSystem::AddAmmo(PickupType type, int newAmmo)
{
	UGunComponent* Weapon = nullptr;

	if (type == PickupType::AMMO_MACHINE_GUN)
	{
		Weapon = AcquiredWeapons.FindRef(WeaponType::MACHINE_GUN);
	}
	if (type == PickupType::AMMO_SHOTGUN)
	{
		Weapon = AcquiredWeapons.FindRef(WeaponType::SHOTGUN);
	}
	if (type == PickupType::AMMO_GRENADE_LAUNCHER)
	{
		Weapon = AcquiredWeapons.FindRef(WeaponType::GRENADE_LAUNCHER);
	}

	if (Weapon != nullptr)
	{
		if (Weapon->Ammo < Weapon->MaxAmmo)
		{
			if ((Weapon->Ammo + newAmmo) > Weapon->MaxAmmo)
			{
				int NoAdd = (Weapon->Ammo + newAmmo) - Weapon->MaxAmmo;
				int Add = newAmmo - NoAdd;
				Weapon->Ammo += Add;
				return true;
			}
			else
			{
				Weapon->Ammo += newAmmo;
				return true;
			}
		}
	}
	
	return false;
}

int AWeaponManagementSystem::GetMaxMagazine()
{
	UGunComponent* Weapon = nullptr;
	int MaxMag = 0;
	if (AcquiredWeapons.Num() > 0)
	{
		Weapon = AcquiredWeapons.FindRef(EquippedWeapon);
		MaxMag = Weapon->MaxMagazine;
	}
	return MaxMag;
}

int AWeaponManagementSystem::GetLoadedMagazine()
{
	UGunComponent* Weapon = nullptr;
	int Mag = 0;
	if (AcquiredWeapons.Num() > 0)
	{
		Weapon = AcquiredWeapons.FindRef(EquippedWeapon);
		Mag = Weapon->Magazine;
	}
	return Mag;
}
