#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP4Testing/Utility/Timer.h"

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
	void Slide(bool& input) noexcept;
	void PlayJumpAudio() noexcept;

public:
	void SetupStartingState() noexcept;

public:
	inline void RegisterPrimaryPlayerReference(APrimaryPlayer& player) noexcept { primaryPlayerRef = &player; }

private:
	void UpdateSlideCameraTransition(float deltaTime);
	void UpdateSlideVelocity();
	void StopSlide() noexcept;
	void StopDash();
	void ResetDash();
	void ResetSlideCooldown() noexcept;
	

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
	float SlideCooldown = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float SlideSpeedDecreaseRate = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float SlideCameraZHeight = -90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float SlideCameraTransitionSpeed = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float CameraTransitionTolerans = 0.5f;
	


	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* JumpAudio;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Debugging")
    bool bCanDash = true;

	UPROPERTY(VisibleAnywhere, Category = "Debugging")
	bool bCanSlide = true;

	UPROPERTY(VisibleAnywhere, Category = "Debugging")
	bool bIsDashing = false;

	UPROPERTY(VisibleAnywhere, Category = "Debugging")
	bool bIsSliding = false;

	UPROPERTY(VisibleAnywhere, Category = "Debugging")
	float CurrentSlideSpeed = 0.0f;

	FTimerHandle DashTimerHandle;
	FTimerHandle DashCooldownTimerHandle;
	Timer slideCooldownTimer;

	bool transitionCameraToSlide = false;
	bool transitionCameraToNormal = false;

private:
	
	APrimaryPlayer* primaryPlayerRef = nullptr;
};