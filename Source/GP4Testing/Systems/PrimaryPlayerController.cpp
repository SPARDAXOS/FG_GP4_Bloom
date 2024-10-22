
#include "GP4Testing/Systems/PrimaryPlayerController.h"
#include "GP4Testing/Systems/PrimaryPlayer.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"

#include "GP4Testing/Utility/Debugging.h"





void APrimaryPlayerController::OnPossess(APawn* aPawn) {
	primaryPlayerRef = Cast<APrimaryPlayer>(aPawn);
	checkf(primaryPlayerRef, TEXT("Primary Player Ref is invalid at PrimaryPlayerController - OnPosses"));

	enhancedInputComponentRef = Cast<UEnhancedInputComponent>(InputComponent);
	checkf(enhancedInputComponentRef, TEXT("Enhanced Input Component Ref is invalid at PrimaryPlayerController - OnPosses"));

	inputSubsystemRef = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	checkf(inputSubsystemRef, TEXT("Input Subsystem Ref is invalid at PrimaryPlayerController - OnPosses"));

	SetupInputMappingContext();
	SetupInputActions();

	Super::OnPossess(aPawn);
}
void APrimaryPlayerController::OnUnPossess() {
	enhancedInputComponentRef->ClearActionBindings();
	Super::OnUnPossess();
}


void APrimaryPlayerController::SetupInputMappingContext() const noexcept {
	inputSubsystemRef->ClearAllMappings();
	inputSubsystemRef->AddMappingContext(defaultContextMappings, 0);
}
void APrimaryPlayerController::SetupInputActions() noexcept {
	checkf(movement, TEXT("movement action is invalid!"));
	checkf(look, TEXT("look action is invalid!"));
	checkf(jump, TEXT("jump action is invalid!"));
	checkf(dash, TEXT("dash action is invalid!"));
	checkf(slide, TEXT("slide action is invalid!"));
	checkf(shoot, TEXT("shoot action is invalid!"));
	checkf(melee, TEXT("melee action is invalid!"));
	checkf(pauseToggle, TEXT("pauseToggle action is invalid!"));
	checkf(reload, TEXT("reload action is invalid!"));
	checkf(switchNextWeapon, TEXT("switchNextWeapon action is invalid!"));
	checkf(switchPreviousWeapon, TEXT("switchPreviousWeapon action is invalid!"));
	checkf(weaponSlot1, TEXT("weaponSlot1 action is invalid!"));
	checkf(weaponSlot2, TEXT("weaponSlot2 action is invalid!"));
	checkf(weaponSlot3, TEXT("weaponSlot3 action is invalid!"));

	enhancedInputComponentRef->BindAction(movement, ETriggerEvent::Triggered, this, &APrimaryPlayerController::HandleMovement);
	enhancedInputComponentRef->BindAction(look, ETriggerEvent::Triggered, this, &APrimaryPlayerController::HandleLook);
	enhancedInputComponentRef->BindAction(jump, ETriggerEvent::Triggered, this, &APrimaryPlayerController::HandleJump);
	enhancedInputComponentRef->BindAction(dash, ETriggerEvent::Triggered, this, &APrimaryPlayerController::HandleDash);
	enhancedInputComponentRef->BindAction(slide, ETriggerEvent::Triggered, this, &APrimaryPlayerController::HandleSlide);
	enhancedInputComponentRef->BindAction(shoot, ETriggerEvent::Triggered, this, &APrimaryPlayerController::HandleShoot);
	enhancedInputComponentRef->BindAction(melee, ETriggerEvent::Triggered, this, &APrimaryPlayerController::HandleMelee);
	enhancedInputComponentRef->BindAction(pauseToggle, ETriggerEvent::Triggered, this, &APrimaryPlayerController::HandlePause);
	enhancedInputComponentRef->BindAction(reload, ETriggerEvent::Triggered, this, &APrimaryPlayerController::HandleReload);
	enhancedInputComponentRef->BindAction(switchNextWeapon, ETriggerEvent::Triggered, this, &APrimaryPlayerController::HandleSwitchNextWeapon);
	enhancedInputComponentRef->BindAction(switchPreviousWeapon, ETriggerEvent::Triggered, this, &APrimaryPlayerController::HandleSwitchPreviousWeapon);
	enhancedInputComponentRef->BindAction(weaponSlot1, ETriggerEvent::Triggered, this, &APrimaryPlayerController::HandleWeaponSlot1);
	enhancedInputComponentRef->BindAction(weaponSlot2, ETriggerEvent::Triggered, this, &APrimaryPlayerController::HandleWeaponSlot2);
	enhancedInputComponentRef->BindAction(weaponSlot3, ETriggerEvent::Triggered, this, &APrimaryPlayerController::HandleWeaponSlot3);

}


void APrimaryPlayerController::SetControllerInputMode(ControllerInputMode mode) noexcept {
	currentControllerInputMode = mode;
	switch (currentControllerInputMode) {
	case ControllerInputMode::NONE:
		SetupOffInputMode();
		break;
	case ControllerInputMode::GAMEPLAY:
		SetupGameplayInputMode();
		break;
	case ControllerInputMode::MENU:
		SetupMenuInputMode();
		break;
	case ControllerInputMode::PAUSED:
		SetupPausedInputMode();
		break;
	default:
		break;
	}
}


void APrimaryPlayerController::SetupGameplayInputMode() noexcept {
	SetPlayerInputState(true);
	SetCursorState(false);
	SetGameInputMode(GameInputMode::GAME_ONLY);
}
void APrimaryPlayerController::SetupMenuInputMode() noexcept {
	SetPlayerInputState(false);
	SetCursorState(true);
	SetGameInputMode(GameInputMode::UI_ONLY);
}
void APrimaryPlayerController::SetupOffInputMode() noexcept {
	SetPlayerInputState(false);
	SetCursorState(false);
	SetGameInputMode(GameInputMode::GAME_ONLY);
}
void APrimaryPlayerController::SetupPausedInputMode() noexcept {
	SetPlayerInputState(true);
	SetCursorState(true);
	SetGameInputMode(GameInputMode::GAME_AND_UI);
}


void APrimaryPlayerController::SetPlayerInputState(bool state) noexcept {
	if (state)
		EnableInput(this);
	else
		DisableInput(this);
}
void APrimaryPlayerController::SetCursorState(bool state) noexcept {
	SetShowMouseCursor(state);
}
void APrimaryPlayerController::SetGameInputMode(GameInputMode mode) noexcept {
	if (mode == GameInputMode::NONE)
		return;

	if (mode == GameInputMode::UI_ONLY)
		SetInputMode(FInputModeUIOnly());
	else if (mode == GameInputMode::GAME_ONLY)
		SetInputMode(FInputModeGameOnly());
	else if (mode == GameInputMode::GAME_AND_UI) {
		FInputModeGameAndUI inputMode;
		inputMode.SetHideCursorDuringCapture(false);
		inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		inputMode.SetWidgetToFocus(nullptr);
		SetInputMode(inputMode);
	}
}

void APrimaryPlayerController::HandleMovement(const FInputActionValue& value) {
	if (currentControllerInputMode == ControllerInputMode::PAUSED || !primaryPlayerRef->GetActiveState())
		return;

	FVector2D axis = value.Get<FVector2D>();
	primaryPlayerRef->HandleMovementInput(axis);
}
void APrimaryPlayerController::HandleLook(const FInputActionValue& value) {
	if (currentControllerInputMode == ControllerInputMode::PAUSED || !primaryPlayerRef->GetActiveState())
		return;

	FVector2D axis = value.Get<FVector2D>();
	primaryPlayerRef->HandleLookInput(axis);
}
void APrimaryPlayerController::HandleJump() {
	if (currentControllerInputMode == ControllerInputMode::PAUSED || !primaryPlayerRef->GetActiveState())
		return;

	primaryPlayerRef->HandleJumpInput();
}
void APrimaryPlayerController::HandleDash() {
	if (currentControllerInputMode == ControllerInputMode::PAUSED || !primaryPlayerRef->GetActiveState())
		return;

	primaryPlayerRef->HandleDashInput();
}
void APrimaryPlayerController::HandleSlide(const FInputActionValue& value) {
	if (currentControllerInputMode == ControllerInputMode::PAUSED || !primaryPlayerRef->GetActiveState())
		return;

	bool input = value.Get<bool>();
	primaryPlayerRef->HandleSlideInput(input);
}
void APrimaryPlayerController::HandleShoot(const FInputActionValue& value) {
	if (currentControllerInputMode == ControllerInputMode::PAUSED || !primaryPlayerRef->GetActiveState())
		return;

	bool input = value.Get<bool>();
	primaryPlayerRef->HandleShootInput(input);
}
void APrimaryPlayerController::HandleMelee() {
	if (currentControllerInputMode == ControllerInputMode::PAUSED || !primaryPlayerRef->GetActiveState())
		return;

	primaryPlayerRef->HandleMeleeInput();
}
void APrimaryPlayerController::HandlePause() {
	if (currentControllerInputMode == ControllerInputMode::PAUSED || !primaryPlayerRef->GetActiveState())
		return;

	primaryPlayerRef->HandlePauseInput();
}
void APrimaryPlayerController::HandleReload() {
	if (currentControllerInputMode == ControllerInputMode::PAUSED || !primaryPlayerRef->GetActiveState())
		return;

	primaryPlayerRef->HandleReloadInput();
}
void APrimaryPlayerController::HandleSwitchNextWeapon() {
	if (currentControllerInputMode == ControllerInputMode::PAUSED || !primaryPlayerRef->GetActiveState())
		return;
	
	primaryPlayerRef->HandleSwitchNextWeaponInput();
}
void APrimaryPlayerController::HandleSwitchPreviousWeapon() {
	if (currentControllerInputMode == ControllerInputMode::PAUSED || !primaryPlayerRef->GetActiveState())
		return;

	primaryPlayerRef->HandleSwitchPreviousWeaponInput();
}
void APrimaryPlayerController::HandleWeaponSlot1() {
	if (currentControllerInputMode == ControllerInputMode::PAUSED || !primaryPlayerRef->GetActiveState())
		return;

	primaryPlayerRef->HandleWeaponSlot1Input();
}
void APrimaryPlayerController::HandleWeaponSlot2() {
	if (currentControllerInputMode == ControllerInputMode::PAUSED || !primaryPlayerRef->GetActiveState())
		return;

	primaryPlayerRef->HandleWeaponSlot2Input();
}
void APrimaryPlayerController::HandleWeaponSlot3() {
	if (currentControllerInputMode == ControllerInputMode::PAUSED || !primaryPlayerRef->GetActiveState())
		return;

	primaryPlayerRef->HandleWeaponSlot3Input();
}
