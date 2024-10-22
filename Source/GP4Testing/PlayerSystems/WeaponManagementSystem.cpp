#include "WeaponManagementSystem.h"

bool AWeaponManagementSystem::UseCurrentWeapon(bool& input) noexcept {
	if (AcquiredWeapons.Num() > 0 && !bSwappingWeapon)
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

bool AWeaponManagementSystem::SwitchNextWeaponTimer()
{
	if (AcquiredWeapons.Num() > 1)
	{
		AGunComponent* Weapon = GetCurrentWeapon();
		if (!Weapon)
		{
			return false;
		}
		Weapon->EndPlay();
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
	return false;
}

void AWeaponManagementSystem::StartWeaponSwap()
{
	bSwappingWeapon = true;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]
		{
			bSwappingWeapon = false;
		});

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.3f, false);
}

bool AWeaponManagementSystem::SwitchPrevWeaponTimer()
{
	if (AcquiredWeapons.Num() > 1)
	{
		AGunComponent* Weapon = GetCurrentWeapon();
		if (!Weapon)
		{
			return false;
		}
		Weapon->EndPlay();
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
	return false;
}

bool AWeaponManagementSystem::WeaponSlotTimer(int32 value)
{
	if (AcquiredWeapons.Num() > value)
	{
		AGunComponent* Weapon = GetCurrentWeapon();
		if (!Weapon)
		{
			return false;
		}
		Weapon->EndPlay();
		Weapon->SetActorHiddenInGame(true);

		TArray<WeaponType> weapons;
		for (auto it = AcquiredWeapons.CreateConstIterator(); it; ++it) {
			weapons.Add(it->Key);
		}

		AGunComponent* NextWeapon = AcquiredWeapons.FindRef(weapons[value]);
		NextWeapon->SetActorHiddenInGame(false);
		EquippedWeapon = weapons[value];
	}
	return false;
}

bool AWeaponManagementSystem::SwitchNextWeapon() noexcept 
{
	StartWeaponSwap();

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]
		{
			GetWorld()->GetTimerManager().ClearTimer(LambdaTimerHandle);
			SwitchNextWeaponTimer();
		});

	GetWorld()->GetTimerManager().SetTimer(LambdaTimerHandle, TimerDelegate, 0.3f, false);

	return true;
}
bool AWeaponManagementSystem::SwitchPreviousWeapon() noexcept 
{
	StartWeaponSwap();

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]
		{
			GetWorld()->GetTimerManager().ClearTimer(LambdaTimerHandle);
			SwitchPrevWeaponTimer();
		});

	GetWorld()->GetTimerManager().SetTimer(LambdaTimerHandle, TimerDelegate, 0.3f, false);

	return true;
}

bool AWeaponManagementSystem::WeaponSlot1() noexcept
{
	StartWeaponSwap();

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]
		{
			GetWorld()->GetTimerManager().ClearTimer(LambdaTimerHandle);
			WeaponSlotTimer(0);
		});

	GetWorld()->GetTimerManager().SetTimer(LambdaTimerHandle, TimerDelegate, 0.3f, false);

	return false;
}

bool AWeaponManagementSystem::WeaponSlot2() noexcept
{
	StartWeaponSwap();

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]
		{
			GetWorld()->GetTimerManager().ClearTimer(LambdaTimerHandle);
			WeaponSlotTimer(1);
		});

	GetWorld()->GetTimerManager().SetTimer(LambdaTimerHandle, TimerDelegate, 0.3f, false);

	return false;
}

bool AWeaponManagementSystem::WeaponSlot3() noexcept
{
	StartWeaponSwap();

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]
		{
			GetWorld()->GetTimerManager().ClearTimer(LambdaTimerHandle);
			WeaponSlotTimer(2);
		});

	GetWorld()->GetTimerManager().SetTimer(LambdaTimerHandle, TimerDelegate, 0.3f, false);
	
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
		Weapon->EndPlay();
		Weapon->SetActorHiddenInGame(true);
	}
	
	AcquiredWeapons.Add(type, weapon);
	EquippedWeapon = type;

	return true;
}
bool AWeaponManagementSystem::HasWeapon(WeaponType type) {
	return AcquiredWeapons.Contains(type);
}

void AWeaponManagementSystem::SetupStartingState() noexcept {
	ClearAllWeapons();

}

void AWeaponManagementSystem::ClearAllWeapons() noexcept
{
	AcquiredWeapons.Reset();
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



