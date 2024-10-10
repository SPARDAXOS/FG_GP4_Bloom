
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
		FVector DashDir = primaryPlayerRef->GetCamera()->GetForwardVector();
		FVector DashVel = DashDir * DashStrength;
		primaryPlayerRef->LaunchCharacter(DashVel, true, true);

		bCanDash = false;


		GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &APlayerMovementSystem::StopDash, DashDuration, false);
		GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &APlayerMovementSystem::ResetDash, DashCooldown, false);
	}
}

void APlayerMovementSystem::SetupStartingState() noexcept {
	currentMovementSpeedModifier = defaultMovementSpeedModifier;
	if (primaryPlayerRef) {
		primaryPlayerRef->GetCharacterMovement()->MaxWalkSpeed = movementSpeed;

	}
}

void APlayerMovementSystem::StopDash()
{
	if (primaryPlayerRef)
	{
		primaryPlayerRef->GetCharacterMovement()->Velocity = FVector::ZeroVector;
	}

}

void APlayerMovementSystem::ResetDash()
{

	bCanDash = true;
}


