
#include "GP4Testing/PlayerSystems/PlayerMovementSystem.h"
#include "GP4Testing/Systems/PrimaryPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"


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
		StoredVelocity = primaryPlayerRef->GetCharacterMovement()->Velocity;

		FVector DashDir = primaryPlayerRef->GetCamera()->GetForwardVector();
		FVector DashVel = DashDir * DashStrength;
		primaryPlayerRef->LaunchCharacter(DashVel, true, true);
		

		bCanDash = false;
		bIsDashing = true;;

		GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &APlayerMovementSystem::StopDash, DashDuration, false);
		
		GetWorld()->GetTimerManager().SetTimer(DashCooldownTimerHandle, this, &APlayerMovementSystem::resetDash, DashCooldown, false);
	}
}

void APlayerMovementSystem::Slide() noexcept
{
	if (bCanSlide && primaryPlayerRef) 
	{
		StoredVelocity = primaryPlayerRef->GetCharacterMovement()->Velocity;

		FVector SlideDir = primaryPlayerRef->GetCamera()->GetForwardVector();
		SlideDir.Z = 0.0f;
		FVector SlideVel = SlideDir * SlideSpeed;
		primaryPlayerRef->LaunchCharacter(SlideVel, true, false);

		bCanSlide = false;
		bIsSliding = true;

		GetWorld()->GetTimerManager().SetTimer(SlideTimerHandle, this, &APlayerMovementSystem::StopSlide, SlideDuration, false);

		GetWorld()->GetTimerManager().SetTimer(SlideCooldownTimerHandle, this, &APlayerMovementSystem::resetSlide, SlideCooldown, false);

	}


}

void APlayerMovementSystem::SetupStartingState() noexcept {
	currentMovementSpeedModifier = defaultMovementSpeedModifier;
	if (primaryPlayerRef) {
		primaryPlayerRef->GetCharacterMovement()->StopActiveMovement();
		primaryPlayerRef->GetCharacterMovement()->MaxWalkSpeed = movementSpeed;

	}

	bCanSlide = true;
	bCanDash = true;
	bIsSliding = false;
	bIsDashing = false;
}

void APlayerMovementSystem::StopSlide()
{
	//primaryPlayerRef->GetCharacterMovement()->Velocity = StoredVelocity;
	
	bIsSliding = false;
}

void APlayerMovementSystem::StopDash()
{
	// primaryPlayerRef->GetCharacterMovement()->Velocity = StoredVelocity;
	
	bIsDashing = false;
}

void APlayerMovementSystem::resetSlide()
{
	bCanSlide = true;
}

void APlayerMovementSystem::resetDash()
{
	bCanDash = true;
}













