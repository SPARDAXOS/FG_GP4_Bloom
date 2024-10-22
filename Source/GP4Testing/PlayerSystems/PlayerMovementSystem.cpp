
#include "GP4Testing/PlayerSystems/PlayerMovementSystem.h"
#include "GP4Testing/Systems/PrimaryPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


APlayerMovementSystem::APlayerMovementSystem() {
	PrimaryActorTick.bCanEverTick = true;
}
void APlayerMovementSystem::Tick(float deltaTime) {
	Super::Tick(deltaTime);

	if (bIsSliding || CurrentSlideSpeed > 0.0f) {
		CurrentSlideSpeed -= SlideSpeedDecreaseRate * deltaTime;
		if (CurrentSlideSpeed <= 0.0f) {
			CurrentSlideSpeed = 0.0f;
			UpdateSlideVelocity();
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
		
		GetWorld()->GetTimerManager().SetTimer(DashCooldownTimerHandle, this, &APlayerMovementSystem::resetDash, DashCooldown, false);
	}
}

void APlayerMovementSystem::Slide() noexcept {
	if (!bIsSliding && CurrentSlideSpeed == 0.0f) {

		CurrentSlideSpeed = SlideSpeed;
		bIsSliding = true;

		FVector cameraLocation = primaryPlayerRef->GetCamera()->GetRelativeLocation();
		cameraLocation.Z = SlideCameraZHeight;
		primaryPlayerRef->GetCamera()->SetRelativeLocation(cameraLocation);
	}
}

void APlayerMovementSystem::SetupStartingState() noexcept {
	currentMovementSpeedModifier = defaultMovementSpeedModifier;
	if (primaryPlayerRef) {
		primaryPlayerRef->GetCharacterMovement()->StopActiveMovement();
		primaryPlayerRef->GetCharacterMovement()->MaxWalkSpeed = movementSpeed;

	}

	bCanDash = true;
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

	FVector cameraLocation = primaryPlayerRef->GetCamera()->GetRelativeLocation();
	cameraLocation.Z = primaryPlayerRef->GetInitialCameraPosition().Z;
	primaryPlayerRef->GetCamera()->SetRelativeLocation(cameraLocation);

	CurrentSlideSpeed = 0.0f;
	bIsSliding = false;
}
void APlayerMovementSystem::StopDash() {
	FVector currentVelocity = primaryPlayerRef->GetCharacterMovement()->Velocity;
	primaryPlayerRef->GetCharacterMovement()->Velocity = currentVelocity * dashRegainedVelocity;
	
	bIsDashing = false;
}
void APlayerMovementSystem::resetDash() {
	bCanDash = true;
}

void APlayerMovementSystem::PlayJumpAudio() noexcept
{
	if (JumpAudio != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, JumpAudio, GetActorLocation());
	}
}













