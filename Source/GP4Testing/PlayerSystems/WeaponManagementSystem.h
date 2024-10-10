#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponTypes.h"
#include <GP4Testing/Weapons/GunComponent.h>


#include "WeaponManagementSystem.generated.h"


class APrimaryPlayer;

UCLASS(Abstract)
class GP4TESTING_API AWeaponManagementSystem : public AActor {
	GENERATED_BODY()

public:
	bool UseCurrentWeapon(bool& input) noexcept; //Called by player to use/shoot current weapon.
	bool ReloadWeapon() noexcept;

public:
	bool SwitchNextWeapon() noexcept;
	bool SwitchPreviousWeapon() noexcept;

	bool WeaponSlot1() noexcept;
	bool WeaponSlot2() noexcept;
	bool WeaponSlot3() noexcept;

public:
	bool AcquireWeapon(WeaponType type, UGunComponent* weapon) noexcept;

public:
	void SetupStartingState() noexcept;

public:
	inline void RegisterPrimaryPlayerReference(APrimaryPlayer& player) noexcept { primaryPlayerRef = &player; }

public:
	UGunComponent* GetCurrentWeapon();
	TMap<WeaponType, UGunComponent*> GetAcquiredWeapons();

	float GetMaxAmmo();
	float GetAmmo();

	bool SetAmmo(float newAmmo);

	float GetMaxMagazine();
	float GetLoadedMagazine();

private:
	APrimaryPlayer* primaryPlayerRef;

	TMap<WeaponType, UGunComponent*> AcquiredWeapons;
	WeaponType EquippedWeapon;
};