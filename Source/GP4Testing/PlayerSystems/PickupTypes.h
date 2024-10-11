#pragma once

#include "PickupTypes.generated.h"


//Add all pickups here!
UENUM(BlueprintType)
enum class PickupType : uint8 {
	NONE = 0		UMETA(DisplayName = "Invalid"),
	HEALTH			UMETA(DisplayName = "Health"),
	AMMO			UMETA(DisplayName = "Ammo"),

	AMMO_MACHINE_GUN		UMETA(DisplayName = "Machine gun ammo"),
	AMMO_SHOTGUN			UMETA(DisplayName = "Shotgun ammo"),
	AMMO_GRENADE_LAUNCHER	UMETA(DisplayName = "Grenade launcher ammo"),
};