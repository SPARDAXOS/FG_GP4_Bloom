#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponTypes.h"


#include "WeaponManagementSystem.generated.h"


class APrimaryPlayer;

UCLASS(Abstract)
class GP4TESTING_API AWeaponManagementSystem : public AActor {
	GENERATED_BODY()

public:
	bool UseCurrentWeapon() noexcept; //Called by player to use current weapon.

public:
	bool SwitchNextWeapon() noexcept;
	bool SwitchPreviousWeapon() noexcept;
	bool AcquireWeapon(WeaponType type) noexcept;

public:
	void SetupStartingState() noexcept;

public:
	inline void RegisterPrimaryPlayerReference(APrimaryPlayer& player) noexcept { primaryPlayerRef = &player; }

public:
	//GetCurrentWeapon()
	//GetLoadedAmmo()
	//GetTotalAmmo()

	void SetHasWeapon(bool hasWeapon);
	bool GetHasWeapon();


private:
	APrimaryPlayer* primaryPlayerRef;

	bool bHasWeapon = false;
};