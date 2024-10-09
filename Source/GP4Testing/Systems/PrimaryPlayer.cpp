
#include "PrimaryPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GP4Testing/Systems/PrimaryGameMode.h"
#include "GP4Testing/PlayerSystems/PlayerMovementSystem.h"
#include "GP4Testing/PlayerSystems/PlayerHealthSystem.h"
#include "GP4Testing/PlayerSystems/PickupManagementSystem.h"
#include "GP4Testing/PlayerSystems/WeaponManagementSystem.h"

#include "GP4Testing/Utility/Debugging.h"


APrimaryPlayer::APrimaryPlayer() {
	PrimaryActorTick.bCanEverTick = true;

}



//Use these instead!
void APrimaryPlayer::Init() {
	CreatePlayerSystems();
	SetupPlayerSystemsDependencies();
	InitPlayerSystems();
}
void APrimaryPlayer::Start() {
	StartPlayerSystems();
}
void APrimaryPlayer::Update(float deltaTime) {

}
void APrimaryPlayer::SetupStartingState() noexcept {
	//Reset all player data to default.
	if (playerMovementSystemRef)
		playerMovementSystemRef->SetupStartingState();

	if (playerHealthSystemRef)
		playerHealthSystemRef->SetupStartingState();

	if (weaponManagementSystemRef)
		weaponManagementSystemRef->SetupStartingState();
}
void APrimaryPlayer::SetPlayerState(bool state) noexcept {
	SetActorTickEnabled(state);
	SetActorEnableCollision(state);
	SetActorHiddenInGame(!state);

	GetCapsuleComponent()->SetEnableGravity(state);
	GetCharacterMovement()->SetActive(state);
	if (state)
		GetCharacterMovement()->GravityScale =	1.0f;	//defaultGravityScale;
	else
		GetCharacterMovement()->GravityScale = 0.0f;

	active = state;
}
void APrimaryPlayer::SetPlayerHUDState(bool state) noexcept {

}


//HandleInput
void APrimaryPlayer::HandleMovementInput(FVector2D axis) noexcept {
	if (!playerMovementSystemRef)
		return;

	playerMovementSystemRef->UpdateMovement(axis);
}
void APrimaryPlayer::HandleLookInput(FVector2D axis) noexcept {
	if (!playerMovementSystemRef)
		return;

	playerMovementSystemRef->UpdateRotation(axis);
}
void APrimaryPlayer::HandleJumpInput() noexcept {
	if (!playerMovementSystemRef)
		return;

	playerMovementSystemRef->Jump();
}
void APrimaryPlayer::HandleShootInput(bool& input) noexcept {
	//input == true => Pressed
	//input == false => Released
	if (!weaponManagementSystemRef)
		return;

	weaponManagementSystemRef->UseCurrentWeapon(input);
}
void APrimaryPlayer::HandlePauseInput() noexcept {
	if (!primaryGameModeRef->GetGamePaused())
		primaryGameModeRef->PauseGame();
	else
		primaryGameModeRef->UnpauseGame();
}
void APrimaryPlayer::HandleReloadInput() noexcept {
	if (!weaponManagementSystemRef)
		return;

	weaponManagementSystemRef->ReloadWeapon();
}
void APrimaryPlayer::HandleSwitchNextWeaponInput() noexcept {

}
void APrimaryPlayer::HandleSwitchPreviousWeaponInput() noexcept {

}
void APrimaryPlayer::HandleWeaponSlot1Input() noexcept {

}
void APrimaryPlayer::HandleWeaponSlot2Input() noexcept {

}
void APrimaryPlayer::HandleWeaponSlot3Input() noexcept {

}



void APrimaryPlayer::CreatePlayerSystems() {
	checkf(playerMovementSystemAsset, TEXT("PlayerMovementSystemAsset ref is null!"));
	playerMovementSystemRef = GetWorld()->SpawnActor<APlayerMovementSystem>(playerMovementSystemAsset);
	checkf(playerMovementSystemRef, TEXT("Failed to create PlayerMovementSystem!"));

	checkf(playerHealthSystemAsset, TEXT("PlayerHealthSystemAsset ref is null!"));
	playerHealthSystemRef = GetWorld()->SpawnActor<APlayerHealthSystem>(playerHealthSystemAsset);
	checkf(playerHealthSystemRef, TEXT("Failed to create PlayerHealthSystem!"));

	checkf(weaponManagementSystemAsset, TEXT("WeaponManagementSystemAsset ref is null!"));
	weaponManagementSystemRef = GetWorld()->SpawnActor<AWeaponManagementSystem>(weaponManagementSystemAsset);
	checkf(weaponManagementSystemRef, TEXT("Failed to create WeaponManagementSystem!"));

	checkf(pickupManagementSystemAsset, TEXT("PickupManagementSystemAsset ref is null!"));
	pickupManagementSystemRef = GetWorld()->SpawnActor<APickupManagementSystem>(pickupManagementSystemAsset);
	checkf(pickupManagementSystemRef, TEXT("Failed to create PickupManagementSystem!"));
}
void APrimaryPlayer::SetupPlayerSystemsDependencies() noexcept {
	if (playerMovementSystemRef) {
		playerMovementSystemRef->RegisterPrimaryPlayerReference(*this);

	}

	if (playerHealthSystemRef) {
		playerHealthSystemRef->RegisterPrimaryPlayerReference(*this);

	}

	if (weaponManagementSystemRef) {
		weaponManagementSystemRef->RegisterPrimaryPlayerReference(*this);

	}

	if (pickupManagementSystemRef) {
		pickupManagementSystemRef->RegisterPrimaryPlayerReference(*this);

	}
}
void APrimaryPlayer::InitPlayerSystems() noexcept {
	//if (playerMovementSystemRef)
	//	playerMovementSystemRef->Init();
	//
	//if (playerHealthSystemRef)
	//	playerHealthSystemRef->Init();
	//
	//if (weaponManagementSystemRef)
	//	weaponManagementSystemRef->Init();
	//
	//if (pickupManagementSystemRef)
	//	pickupManagementSystemRef->Init();
}
void APrimaryPlayer::StartPlayerSystems() noexcept {
	//if (playerMovementSystemRef)
	//	playerMovementSystemRef->Start();
	//
	//if (playerHealthSystemRef)
	//	playerHealthSystemRef->Start();
	//
	//if (weaponManagementSystemRef)
	//	weaponManagementSystemRef->Start();
	//
	//if (pickupManagementSystemRef)
	//	pickupManagementSystemRef->Start();
}
