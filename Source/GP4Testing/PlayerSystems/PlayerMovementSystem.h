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
	void Dash() noexcept;

public:
	void SetupStartingState() noexcept;

public:
	inline void RegisterPrimaryPlayerReference(APrimaryPlayer& player) noexcept { primaryPlayerRef = &player; }

private:
	void StopDash();
	void ResetDash();

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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashStrength = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashDuration = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashCooldown = 1.0f;

private:
    bool bCanDash = true;
	FTimerHandle DashTimerHandle;
	FTimerHandle CooldownTimerHandle;

private:
	APrimaryPlayer* primaryPlayerRef = nullptr;
};