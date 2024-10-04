#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP4Testing/Components/HealthComponent.h"
#include "GP4Testing/Systems/Entity.h"


#include "PlayerHealthSystem.generated.h"


class APrimaryPlayer;

UCLASS(Abstract)
class GP4TESTING_API APlayerHealthSystem : public AActor, public Entity {
	GENERATED_BODY()

	APlayerHealthSystem();
	
public:
	inline void RegisterPrimaryPlayerReference(APrimaryPlayer& player) noexcept { primaryPlayerRef = &player; }

public:
	void SetupStartingState() noexcept;

	UHealthComponent* HealthComponent = nullptr;
private:
	APrimaryPlayer* primaryPlayerRef;

	UFUNCTION()
	void KillPlayer();
};