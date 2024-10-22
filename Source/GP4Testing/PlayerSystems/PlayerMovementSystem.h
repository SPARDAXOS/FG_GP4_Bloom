#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


#include "PlayerMovementSystem.generated.h"


class APrimaryPlayer;

UCLASS(Abstract)
class GP4TESTING_API APlayerMovementSystem : public AActor {
	GENERATED_BODY()

public:
	APlayerMovementSystem();

public:
	virtual void Tick(float deltaTime) override;

public:
	void UpdateMovement(FVector2D axis) noexcept;
	void UpdateRotation(FVector2D axis) noexcept;
	void Jump() noexcept;
	void Dash() noexcept;
	void Slide() noexcept;
	void PlayJumpAudio() noexcept;

public:
	void SetupStartingState() noexcept;

public:
	inline void RegisterPrimaryPlayerReference(APrimaryPlayer& player) noexcept { primaryPlayerRef = &player; }

private:
	void UpdateSlideVelocity();
	void StopSlide() noexcept;
	void StopDash();
	void resetDash();
	

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
	float dashRegainedVelocity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashStrength = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashDuration = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashCooldown = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashZOffset = 10.0f;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float SlideRegainedVelocity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float SlideSpeed = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float SlideSpeedDecreaseRate = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float SlideCameraZHeight = -90.0f;


	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* JumpAudio;

private:
    bool bCanDash = true;
	bool bIsDashing = false;
	bool bIsSliding = false;

	float CurrentSlideSpeed = 0.0f;

	FTimerHandle DashTimerHandle;
	FTimerHandle DashCooldownTimerHandle;

private:
	
	APrimaryPlayer* primaryPlayerRef = nullptr;
};