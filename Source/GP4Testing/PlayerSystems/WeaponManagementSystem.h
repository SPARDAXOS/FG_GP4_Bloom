#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponTypes.h"
#include <GP4Testing/Weapons/GunComponent.h>
#include "PickupTypes.h"


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
	bool AcquireWeapon(WeaponType type, AGunComponent* weapon) noexcept;
	bool HasWeapon(WeaponType type);

public:
	void SetupStartingState() noexcept;
	void ClearAllWeapons() noexcept;

public:
	inline void RegisterPrimaryPlayerReference(APrimaryPlayer& player) noexcept { primaryPlayerRef = &player; }

public:
	UFUNCTION(BlueprintCallable)
	AGunComponent* GetCurrentWeapon();

	TMap<WeaponType, AGunComponent*> GetAcquiredWeapons();

	int GetMaxAmmo();
	int GetAmmo();

	bool AddAmmo(PickupType type, int newAmmo);

	int GetMaxMagazine();
	int GetLoadedMagazine();

private:
	APrimaryPlayer* primaryPlayerRef;

	TMap<WeaponType, AGunComponent*> AcquiredWeapons;
	WeaponType EquippedWeapon;

private:
	FTimerHandle WeaponSwitchTimer;
	FTimerHandle SwapTimerHandle;
	FTimerHandle LambdaTimerHandle;

	bool SwitchNextWeaponTimer();
	bool SwitchPrevWeaponTimer();
	bool WeaponSlotTimer(int32 MapValue, int32 weaponIndex);

public:
	void StartWeaponSwap();

	UPROPERTY(BlueprintReadWrite)
	bool bSwappingWeapon = false;
};