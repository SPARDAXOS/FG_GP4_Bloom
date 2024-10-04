#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


#include "PlayerMovementSystem.generated.h"


class APrimaryPlayer;

UCLASS(Abstract)
class GP4TESTING_API APlayerMovementSystem : public AActor {
	GENERATED_BODY()

public:
	void UpdateMovement(FVector2D axis) noexcept;
	void UpdateRotation(FVector2D axis) noexcept;
	void Jump() noexcept;

public:
	void SetupStartingState() noexcept;

public:
	inline void RegisterPrimaryPlayerReference(APrimaryPlayer& player) noexcept { primaryPlayerRef = &player; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float movementSpeed = 400.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float defaultMovementSpeedModifier = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Rotation", meta = (AllowPrivateAccess = "true"))
	float lookSensitivityModifier = 1.0f;

private:
	UPROPERTY(VisibleAnywhere, Category = "Debugging", meta = (AllowPrivateAccess = "true"))
	float currentMovementSpeedModifier = 1.0f;

private:
	APrimaryPlayer* primaryPlayerRef = nullptr;
};