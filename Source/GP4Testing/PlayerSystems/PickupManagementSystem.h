#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupTypes.h"

#include "PickupManagementSystem.generated.h"



class APrimaryPlayer;


UCLASS(Abstract)
class GP4TESTING_API APickupManagementSystem : public AActor {

	GENERATED_BODY()

public:
	bool RegisterPickup(const PickupType& type, float amount) noexcept; //player calls this simply with pickup data.

public:
	inline void RegisterPrimaryPlayerReference(APrimaryPlayer& player) noexcept { primaryPlayerRef = &player; }


private:
	APrimaryPlayer* primaryPlayerRef = nullptr;


};