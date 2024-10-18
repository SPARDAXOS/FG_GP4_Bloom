#include "WeaponManagementSystem.h"

bool AWeaponManagementSystem::UseCurrentWeapon(bool& input) noexcept {
	if (AcquiredWeapons.Num() > 0)
	{
		AGunComponent* Weapon = GetCurrentWeapon();

		if (!Weapon)
		{
			return false;
		}

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
		AGunComponent* Weapon = GetCurrentWeapon();
		if (!Weapon)
		{
			return false;
		}
		Weapon->Reload();
	}
	
	return true;
}


bool AWeaponManagementSystem::SwitchNextWeapon() noexcept {
	if (AcquiredWeapons.Num() > 1)
	{
		AGunComponent* Weapon = GetCurrentWeapon();
		if (!Weapon)
		{
			return false;
		}
		Weapon->ClearWeaponTimer();
		Weapon->StopFire();
		Weapon->SetActorHiddenInGame(true);

		TArray<WeaponType> weapons;
		AcquiredWeapons.GetKeys(weapons);

		int32 CurrentIndex = weapons.Find(EquippedWeapon);

		if (CurrentIndex != weapons.Num() - 1)
		{
			int32 NextIndex = CurrentIndex + 1;
			EquippedWeapon = weapons[NextIndex];
			AGunComponent* NextWeapon = AcquiredWeapons.FindRef(EquippedWeapon);
			NextWeapon->SetActorHiddenInGame(false);
		}
		else if (CurrentIndex == weapons.Num() - 1)
		{
			EquippedWeapon = weapons[0];
			AGunComponent* NextWeapon = AcquiredWeapons.FindRef(EquippedWeapon);
			NextWeapon->SetActorHiddenInGame(false);
		}
	}

	return true;
}
bool AWeaponManagementSystem::SwitchPreviousWeapon() noexcept {
	if (AcquiredWeapons.Num() > 1)
	{
		AGunComponent* Weapon = GetCurrentWeapon();
		if (!Weapon)
		{
			return false;
		}
		Weapon->ClearWeaponTimer();
		Weapon->StopFire();
		Weapon->SetActorHiddenInGame(true);

		TArray<WeaponType> weapons;
		AcquiredWeapons.GetKeys(weapons);

		int32 CurrentIndex = weapons.Find(EquippedWeapon);

		if (CurrentIndex != 0)
		{
			int32 NextIndex = CurrentIndex - 1;
			EquippedWeapon = weapons[NextIndex];
			AGunComponent* NextWeapon = AcquiredWeapons.FindRef(EquippedWeapon);
			NextWeapon->SetActorHiddenInGame(false);
		}
		else if (CurrentIndex == 0)
		{
			EquippedWeapon = weapons[weapons.Num() - 1];
			AGunComponent* NextWeapon = AcquiredWeapons.FindRef(EquippedWeapon);
			NextWeapon->SetActorHiddenInGame(false);
		}
	}

	return true;
}

bool AWeaponManagementSystem::WeaponSlot1() noexcept
{
	if (AcquiredWeapons.Num() > 0)
	{
		AGunComponent* Weapon = GetCurrentWeapon();
		if (!Weapon)
		{
			return false;
		}
		Weapon->StopFire();
		Weapon->SetActorHiddenInGame(true);

		TArray<WeaponType> weapons;
		for (auto it = AcquiredWeapons.CreateConstIterator(); it; ++it) {
			weapons.Add(it->Key);
		}

		AGunComponent* NextWeapon = AcquiredWeapons.FindRef(weapons[0]);
		NextWeapon->SetActorHiddenInGame(false);
		EquippedWeapon = weapons[0];
	}
	
	return false;
}

bool AWeaponManagementSystem::WeaponSlot2() noexcept
{
	if (AcquiredWeapons.Num() > 1)
	{
		AGunComponent* Weapon = GetCurrentWeapon();
		if (!Weapon)
		{
			return false;
		}
		Weapon->StopFire();
		Weapon->SetActorHiddenInGame(true);

		TArray<WeaponType> weapons;
		for (auto it = AcquiredWeapons.CreateConstIterator(); it; ++it) {
			weapons.Add(it->Key);
		}

		AGunComponent* NextWeapon = AcquiredWeapons.FindRef(weapons[1]);
		NextWeapon->SetActorHiddenInGame(false);
		EquippedWeapon = weapons[1];
	}
	
	return false;
}

bool AWeaponManagementSystem::WeaponSlot3() noexcept
{
	if (AcquiredWeapons.Num() > 2)
	{
		AGunComponent* Weapon = GetCurrentWeapon();
		if (!Weapon)
		{
			return false;
		}
		Weapon->StopFire();
		Weapon->SetActorHiddenInGame(true);

		TArray<WeaponType> weapons;
		for (auto it = AcquiredWeapons.CreateConstIterator(); it; ++it) {
			weapons.Add(it->Key);
		}

		AGunComponent* NextWeapon = AcquiredWeapons.FindRef(weapons[2]);
		NextWeapon->SetActorHiddenInGame(false);
		EquippedWeapon = weapons[2];
	}
	
	return false;
}


bool AWeaponManagementSystem::AcquireWeapon(WeaponType type, AGunComponent* weapon) noexcept {
	if (AcquiredWeapons.Num() >= 1)
	{
		// Hide current weapon
		AGunComponent* Weapon = GetCurrentWeapon();
		if (!Weapon)
		{
			return false;
		}
		Weapon->SetActorHiddenInGame(true);
	}
	
	AcquiredWeapons.Add(type, weapon);
	EquippedWeapon = type;

	return true;
}


void AWeaponManagementSystem::SetupStartingState() noexcept {}

void AWeaponManagementSystem::ClearAllWeapons() noexcept
{
	TMap<WeaponType, AGunComponent*> Reset;
	AcquiredWeapons = Reset;
	EquippedWeapon = WeaponType::NONE;
}

AGunComponent* AWeaponManagementSystem::GetCurrentWeapon()
{
	AGunComponent* Weapon = nullptr;
	if (AcquiredWeapons.Num() > 0)
	{
		Weapon = AcquiredWeapons.FindRef(EquippedWeapon);
	}
	return Weapon;
}

TMap<WeaponType, AGunComponent*> AWeaponManagementSystem::GetAcquiredWeapons()
{
	return AcquiredWeapons;
}

int AWeaponManagementSystem::GetMaxAmmo()
{
	AGunComponent* Weapon = GetCurrentWeapon();
	if (!Weapon)
	{
		return false;
	}
	int MaxAmmo = 0;
	if (AcquiredWeapons.Num() > 0)
	{
		MaxAmmo = Weapon->MaxAmmo;
	}
	return MaxAmmo;
}

int AWeaponManagementSystem::GetAmmo()
{
	AGunComponent* Weapon = GetCurrentWeapon();
	if (!Weapon)
	{
		return false;
	}
	int Ammo = 0;
	if (AcquiredWeapons.Num() > 0)
	{
		Ammo = Weapon->Ammo;
	}
	return Ammo;
}

bool AWeaponManagementSystem::AddAmmo(PickupType type, int newAmmo)
{
	AGunComponent* Weapon = nullptr;

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
	AGunComponent* Weapon = GetCurrentWeapon();
	if (!Weapon)
	{
		return false;
	}
	int MaxMag = 0;
	if (AcquiredWeapons.Num() > 0)
	{
		MaxMag = Weapon->MaxMagazine;
	}
	return MaxMag;
}

int AWeaponManagementSystem::GetLoadedMagazine()
{
	AGunComponent* Weapon = GetCurrentWeapon();
	if (!Weapon)
	{
		return false;
	}
	int Mag = 0;
	if (AcquiredWeapons.Num() > 0)
	{
		Mag = Weapon->Magazine;
	}
	return Mag;
}

