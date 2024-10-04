
#include "GP4Testing/PlayerSystems/PlayerMovementSystem.h"
#include "GP4Testing/Systems/PrimaryPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"


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

void APlayerMovementSystem::SetupStartingState() noexcept {
	currentMovementSpeedModifier = defaultMovementSpeedModifier;
	if (primaryPlayerRef) {
		primaryPlayerRef->GetCharacterMovement()->MaxWalkSpeed = movementSpeed;

	}
}