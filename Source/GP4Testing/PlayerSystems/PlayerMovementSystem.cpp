
#include "GP4Testing/PlayerSystems/PlayerMovementSystem.h"
#include "GP4Testing/Systems/PrimaryPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

#include <functional>


APlayerMovementSystem::APlayerMovementSystem() {
	PrimaryActorTick.bCanEverTick = true;

	slideCooldownTimer.SetOnCompletedCallback(std::bind(&APlayerMovementSystem::ResetSlideCooldown, this));
	slideCooldownTimer.SetLengthRef(&SlideCooldown);
}
void APlayerMovementSystem::Tick(float deltaTime) {
	Super::Tick(deltaTime);


	UpdateSlideCameraTransition(deltaTime);

	if (!bIsSliding && CurrentSlideSpeed == 0.0f && !bCanSlide)
		slideCooldownTimer.Update(deltaTime);

	if (bIsSliding || CurrentSlideSpeed > 0.0f) {
		CurrentSlideSpeed -= SlideSpeedDecreaseRate * deltaTime;
		if (CurrentSlideSpeed <= 0.0f) {
			StopSlide();
			return;
		}

		UpdateSlideVelocity();
	}
}



void APlayerMovementSystem::UpdateMovement(FVector2D axis) noexcept {
	if (!primaryPlayerRef)
		return;

	primaryPlayerRef->AddMovementInput(primaryPlayerRef->GetActorForwardVector(), axis.Y);
	primaryPlayerRef->AddMovementInput(primaryPlayerRef->GetActorRightVector(), axis.X);
}
void APlayerMovementSystem::UpdateRotation(FVector2D axis) noexcept {
	if (!primaryPlayerRef)
		return;

	primaryPlayerRef->AddControllerYawInput(axis.X * lookSensitivityModifier);
	primaryPlayerRef->AddControllerPitchInput(axis.Y * lookSensitivityModifier);
}
void APlayerMovementSystem::Jump() noexcept {
	if (!primaryPlayerRef)
		return;

	primaryPlayerRef->Jump();
}

void APlayerMovementSystem::Dash() noexcept {
	if (primaryPlayerRef->GetCharacterMovement()->IsMovingOnGround())
		return;

	if (bCanDash && primaryPlayerRef)
	{

		FVector DashDir = primaryPlayerRef->GetCamera()->GetForwardVector();
		DashDir.Z += DashZOffset;
		FVector DashVel = DashDir * DashStrength * SlideSpeedMultiplier;
		//primaryPlayerRef->LaunchCharacter(DashVel, true, true);
		primaryPlayerRef->GetCharacterMovement()->AddImpulse(DashVel);

		bCanDash = false;
		bIsDashing = true;;

		GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &APlayerMovementSystem::StopDash, DashDuration, false);

		GetWorld()->GetTimerManager().SetTimer(DashCooldownTimerHandle, this, &APlayerMovementSystem::ResetDash, DashCooldown, false);
	}
}

void APlayerMovementSystem::Slide(bool& input) noexcept {
	if (bIsDashing)
		return;

	if (!primaryPlayerRef->GetCharacterMovement()->IsMovingOnGround())
		return;

	if (input && bCanSlide && !bIsSliding && CurrentSlideSpeed == 0.0f) {
		CurrentSlideSpeed = SlideSpeed;
		currentCameraTransitionSpeed = SlideCameraTransitionSpeed;

		bIsSliding = true;
		bCanSlide = false;

		transitionCameraToSlide = true;
		transitionCameraToNormal = false;
	}
	else if (bIsSliding && CurrentSlideSpeed > 0.0f) {
		CurrentSlideSpeed *= SlideSpeedCancelRate;
		if (CurrentSlideSpeed < 0.0f)
			CurrentSlideSpeed = 0.0f;
		currentCameraTransitionReverseSpeed = SlideCameraTransitionCancelSpeed;
	}
}

void APlayerMovementSystem::SetupStartingState() noexcept {
	currentMovementSpeedModifier = defaultMovementSpeedModifier;
	if (primaryPlayerRef) {
		primaryPlayerRef->GetCharacterMovement()->StopActiveMovement();
		primaryPlayerRef->GetCharacterMovement()->MaxWalkSpeed = movementSpeed;

	}

	slideCooldownTimer.ResetTime();
	bCanDash = true;
	bCanSlide = true;
	bIsSliding = false;
	bIsDashing = false;
	CurrentSlideSpeed = 0.0f;
	currentCameraTransitionSpeed = 0.0f;
	currentCameraTransitionReverseSpeed = 0.0f;
}


void APlayerMovementSystem::UpdateSlideCameraTransition(float deltaTime) {
	if (transitionCameraToSlide) {
		FVector currentLocation = primaryPlayerRef->GetCamera()->GetRelativeLocation();
		FVector targetLocation = currentLocation;
		targetLocation.Z = SlideCameraZHeight;

		FVector result = FMath::Lerp(currentLocation, targetLocation, currentCameraTransitionSpeed * deltaTime);
		if (FVector::Distance(targetLocation, result) <= CameraTransitionTolerans) {
			result = targetLocation;
			transitionCameraToSlide = false;
			currentCameraTransitionSpeed = 0.0f;
		}

		primaryPlayerRef->GetCamera()->SetRelativeLocation(result);
	}
	else if (transitionCameraToNormal) {
		FVector currentLocation = primaryPlayerRef->GetCamera()->GetRelativeLocation();
		FVector targetLocation = primaryPlayerRef->GetInitialCameraPosition();

		FVector result = FMath::Lerp(currentLocation, targetLocation, currentCameraTransitionReverseSpeed * deltaTime);
		if (FVector::Distance(primaryPlayerRef->GetInitialCameraPosition(), result) <= CameraTransitionTolerans) {
			result = primaryPlayerRef->GetInitialCameraPosition();
			transitionCameraToNormal = false;
			currentCameraTransitionReverseSpeed = 0.0f;
		}

		primaryPlayerRef->GetCamera()->SetRelativeLocation(result);
	}
}
void APlayerMovementSystem::UpdateSlideVelocity() {
	FVector SlideDir = primaryPlayerRef->GetCamera()->GetForwardVector();
	FVector SlideVel = SlideDir * CurrentSlideSpeed;
	SlideVel.Z = primaryPlayerRef->GetCharacterMovement()->Velocity.Z;
	primaryPlayerRef->GetCharacterMovement()->Velocity = SlideVel;
}
void APlayerMovementSystem::StopSlide() noexcept {
	CurrentSlideSpeed = 0.0f;
	currentCameraTransitionSpeed = 0.0f;
	if (currentCameraTransitionReverseSpeed == 0.0f)
		currentCameraTransitionReverseSpeed = SlideCameraTransitionReverseSpeed;
	UpdateSlideVelocity();

	transitionCameraToSlide = false;
	transitionCameraToNormal = true;
	bIsSliding = false;
}
void APlayerMovementSystem::StopDash() {
	FVector currentVelocity = primaryPlayerRef->GetCharacterMovement()->Velocity;
	primaryPlayerRef->GetCharacterMovement()->Velocity = currentVelocity * dashRegainedVelocity;
	
	bIsDashing = false;
}
void APlayerMovementSystem::ResetDash() {
	bCanDash = true;
}
void APlayerMovementSystem::ResetSlideCooldown() noexcept {
	bCanSlide = true;
}


void APlayerMovementSystem::PlayJumpAudio() noexcept
{
	if (JumpAudio != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, JumpAudio, GetActorLocation());
	}
}
bool APlayerMovementSystem::GetCanDash() {
	return bCanDash;
}
bool APlayerMovementSystem::GetCanSlide() {
	return bCanSlide;
}













