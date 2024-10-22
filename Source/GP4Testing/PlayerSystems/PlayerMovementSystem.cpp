
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

void APlayerMovementSystem::Dash() noexcept
{
	if (bCanDash && primaryPlayerRef)
	{

		FVector DashDir = primaryPlayerRef->GetCamera()->GetForwardVector();
		DashDir.Z += DashZOffset;
		FVector DashVel = DashDir * DashStrength;
		primaryPlayerRef->LaunchCharacter(DashVel, true, true);
		

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
		bIsSliding = true;
		bCanSlide = false;

		FVector cameraLocation = primaryPlayerRef->GetCamera()->GetRelativeLocation();
		cameraLocation.Z = SlideCameraZHeight;
		primaryPlayerRef->GetCamera()->SetRelativeLocation(cameraLocation);
	}
	else if (bIsSliding && CurrentSlideSpeed > 0.0f)
		StopSlide();
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
}


void APlayerMovementSystem::UpdateSlideVelocity() {
	FVector SlideDir = primaryPlayerRef->GetCamera()->GetForwardVector();
	FVector SlideVel = SlideDir * CurrentSlideSpeed;
	SlideVel.Z = primaryPlayerRef->GetCharacterMovement()->Velocity.Z;
	primaryPlayerRef->GetCharacterMovement()->Velocity = SlideVel;
}
void APlayerMovementSystem::StopSlide() noexcept {
	CurrentSlideSpeed = 0.0f;
	UpdateSlideVelocity();

	FVector cameraLocation = primaryPlayerRef->GetCamera()->GetRelativeLocation();
	cameraLocation.Z = primaryPlayerRef->GetInitialCameraPosition().Z;
	primaryPlayerRef->GetCamera()->SetRelativeLocation(cameraLocation);

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













