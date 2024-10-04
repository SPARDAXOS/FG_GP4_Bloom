#pragma once

#include "WeaponTypes.generated.h"


//Add all weapons here!
UENUM(BlueprintType)
enum class WeaponType : uint8 {
	NONE = 0						UMETA(DisplayName = "Invalid"),
	MACHINE_GUN						UMETA(DisplayName = "Machine Gun"),
	SHOTGUN							UMETA(DisplayName = "Shotgun"),
	GRENADE_LAUNCHER				UMETA(DisplayName = "Grenade Launcher"),



};