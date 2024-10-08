
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
	checkf(shoot, TEXT("shoot action is invalid!"));
	checkf(pauseToggle, TEXT("pauseToggle action is invalid!"));
	checkf(reload, TEXT("pauseToggle action is invalid!"));

	enhancedInputComponentRef->BindAction(movement, ETriggerEvent::Triggered, this, &APrimaryPlayerController::HandleMovement);
	enhancedInputComponentRef->BindAction(look, ETriggerEvent::Triggered, this, &APrimaryPlayerController::HandleLook);
	enhancedInputComponentRef->BindAction(jump, ETriggerEvent::Triggered, this, &APrimaryPlayerController::HandleJump);
	enhancedInputComponentRef->BindAction(shoot, ETriggerEvent::Triggered, this, &APrimaryPlayerController::HandleShoot);
	enhancedInputComponentRef->BindAction(pauseToggle, ETriggerEvent::Triggered, this, &APrimaryPlayerController::HandlePause);
	enhancedInputComponentRef->BindAction(reload, ETriggerEvent::Triggered, this, &APrimaryPlayerController::HandleReload);

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
	if (currentControllerInputMode == ControllerInputMode::PAUSED)
		return;

	FVector2D axis = value.Get<FVector2D>();
	primaryPlayerRef->HandleMovementInput(axis);
}
void APrimaryPlayerController::HandleLook(const FInputActionValue& value) {
	if (currentControllerInputMode == ControllerInputMode::PAUSED)
		return;

	FVector2D axis = value.Get<FVector2D>();
	primaryPlayerRef->HandleLookInput(axis);
}
void APrimaryPlayerController::HandleJump() {
	if (currentControllerInputMode == ControllerInputMode::PAUSED)
		return;

	primaryPlayerRef->HandleJumpInput();
}
void APrimaryPlayerController::HandleShoot(const FInputActionValue& value) {
	if (currentControllerInputMode == ControllerInputMode::PAUSED)
		return;

	bool input = value.Get<bool>();
	primaryPlayerRef->HandleShootInput(input);
}
void APrimaryPlayerController::HandlePause() {
	if (currentControllerInputMode == ControllerInputMode::PAUSED)
		return;

	primaryPlayerRef->HandlePauseInput();
}
void APrimaryPlayerController::HandleReload() {
	if (currentControllerInputMode == ControllerInputMode::PAUSED)
		return;

	primaryPlayerRef->HandleReloadInput();
}
