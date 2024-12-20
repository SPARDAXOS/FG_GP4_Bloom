
#include "GP4Testing/Systems/PrimaryPlayerController.h"
#include "GP4Testing/Systems/PrimaryPlayer.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerMappableKeySettings.h"

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

	CacheKeybindings();
	Super::OnPossess(aPawn);
}
void APrimaryPlayerController::OnUnPossess() {
	enhancedInputComponentRef->ClearActionBindings();
	Super::OnUnPossess();
}
void APrimaryPlayerController::BeginPlay() {
	Super::BeginPlay();
	SetupMovementModifiers();
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
	enhancedInputComponentRef->BindAction(pauseToggle, ETriggerEvent::Triggered, this, &APrimaryPlayerController::HandlePause);
	enhancedInputComponentRef->BindAction(reload, ETriggerEvent::Triggered, this, &APrimaryPlayerController::HandleReload);
	enhancedInputComponentRef->BindAction(switchNextWeapon, ETriggerEvent::Triggered, this, &APrimaryPlayerController::HandleSwitchNextWeapon);
	enhancedInputComponentRef->BindAction(switchPreviousWeapon, ETriggerEvent::Triggered, this, &APrimaryPlayerController::HandleSwitchPreviousWeapon);
	enhancedInputComponentRef->BindAction(weaponSlot1, ETriggerEvent::Triggered, this, &APrimaryPlayerController::HandleWeaponSlot1);
	enhancedInputComponentRef->BindAction(weaponSlot2, ETriggerEvent::Triggered, this, &APrimaryPlayerController::HandleWeaponSlot2);
	enhancedInputComponentRef->BindAction(weaponSlot3, ETriggerEvent::Triggered, this, &APrimaryPlayerController::HandleWeaponSlot3);

}


void APrimaryPlayerController::SetupMovementModifiers() noexcept {
	
	forwardMovementSwizzler = NewObject<UInputModifierSwizzleAxis>(defaultContextMappings->GetClass(), TEXT("ForwardSwizzler"));
	forwardMovementSwizzler->Order = EInputAxisSwizzle::YXZ;

	leftMovementNegate = NewObject<UInputModifierNegate>(defaultContextMappings->GetClass(), TEXT("LeftNegate"));
	leftMovementNegate->bX = true;
	leftMovementNegate->bY = true;
	leftMovementNegate->bZ = true;

	backwardMovementSwizzler = NewObject<UInputModifierSwizzleAxis>(defaultContextMappings->GetClass(), TEXT("BackwardSwizzler"));
	backwardMovementSwizzler->Order = EInputAxisSwizzle::YXZ;

	backwardMovementNegate = NewObject<UInputModifierNegate>(defaultContextMappings->GetClass(), TEXT("BackwardNegate"));
	backwardMovementNegate->bX = true;
	backwardMovementNegate->bY = true;
	backwardMovementNegate->bZ = true;
}
void APrimaryPlayerController::CacheKeybindings() noexcept {
	
	
	auto movementMappings = FindMappings(movement);
	//Upon rebind the asset is modified permenentaly and the input bindings positions are changed. 
	//This picks the correct binding based on the modifiers equipped.
	//Important Note: The playerInputSettings? are overridable on each binding and can be used to mark each binding with a name. Thats the ideal solution!
	for (auto& input : movementMappings) {
		if (input->Modifiers.Num() == 0)
			rightKey = input->Key;
		else if (input->Modifiers.Num() == 1) {
			if (Cast<UInputModifierSwizzleAxis>(input->Modifiers[0]))
				forwardKey = movementMappings[0]->Key;
			else if (Cast<UInputModifierNegate>(input->Modifiers[0]))
				leftKey = movementMappings[1]->Key;
		}
		else if (input->Modifiers.Num() == 2)
			backwardKey = movementMappings[2]->Key;
	}


	auto dashMappings = FindMappings(dash);
	dashKey = dashMappings[0]->Key;

	auto slideMappings = FindMappings(slide);
	slideKey = slideMappings[0]->Key;

	auto jumpMappings = FindMappings(jump);
	jumpKey = jumpMappings[0]->Key;

	auto shootMappings = FindMappings(shoot);
	shootKey = shootMappings[0]->Key;

	auto weaponSwitchUpMappings = FindMappings(switchNextWeapon);
	weaponSwitchUpKey = weaponSwitchUpMappings[0]->Key;

	auto weaponSwitchDownMappings = FindMappings(switchPreviousWeapon);
	weaponSwitchDownKey = weaponSwitchDownMappings[0]->Key;

	auto reloadMappings = FindMappings(reload);
	reloadKey = reloadMappings[0]->Key;
}
void APrimaryPlayerController::RebuildContextMappings() noexcept {
	FModifyContextOptions options;
	inputSubsystemRef->RequestRebuildControlMappings(options, EInputMappingRebuildType::RebuildWithFlush);
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


void APrimaryPlayerController::SetForwardKey(FKey key) noexcept {
	auto mapping = FindMapping(forwardKey);
	if (!mapping)
		return;

	defaultContextMappings->UnmapKey(movement, forwardKey);
	FEnhancedActionKeyMapping* keyData = &defaultContextMappings->MapKey(movement, key);
	keyData->Modifiers.Emplace(forwardMovementSwizzler); //This is the reason of the first rebind on game start crash

	RebuildContextMappings();
	forwardKey = key;
}
void APrimaryPlayerController::SetBackwardKey(FKey key) noexcept {
	auto mapping = FindMapping(backwardKey);
	if (!mapping)
		return;

	defaultContextMappings->UnmapKey(movement, backwardKey);
	FEnhancedActionKeyMapping* keyData = &defaultContextMappings->MapKey(movement, key);
	keyData->Modifiers.Add(backwardMovementSwizzler);
	keyData->Modifiers.Add(backwardMovementNegate);

	RebuildContextMappings();
	backwardKey = key;
}
void APrimaryPlayerController::SetRightKey(FKey key) noexcept {
	auto mapping = FindMapping(rightKey);
	if (!mapping)
		return;

	defaultContextMappings->UnmapKey(movement, rightKey);
	FEnhancedActionKeyMapping* keyData = &defaultContextMappings->MapKey(movement, key);

	RebuildContextMappings();
	rightKey = key;
}
void APrimaryPlayerController::SetLeftKey(FKey key) noexcept {
	auto mapping = FindMapping(leftKey);
	if (!mapping)
		return;

	defaultContextMappings->UnmapKey(movement, leftKey);
	FEnhancedActionKeyMapping* keyData = &defaultContextMappings->MapKey(movement, key);
	keyData->Modifiers.Add(leftMovementNegate);

	RebuildContextMappings();
	leftKey = key;
}
void APrimaryPlayerController::SetDashKey(FKey key) noexcept {
	auto mapping = FindMapping(dashKey);
	if (!mapping)
		return;

	defaultContextMappings->UnmapKey(dash, dashKey);
	FEnhancedActionKeyMapping* keyData = &defaultContextMappings->MapKey(dash, key);

	RebuildContextMappings();
	dashKey = key;
}
void APrimaryPlayerController::SetSlideKey(FKey key) noexcept {
	auto mapping = FindMapping(slideKey);
	if (!mapping)
		return;

	defaultContextMappings->UnmapKey(slide, slideKey);
	FEnhancedActionKeyMapping* keyData = &defaultContextMappings->MapKey(slide, key);

	RebuildContextMappings();
	slideKey = key;
}
void APrimaryPlayerController::SetJumpKey(FKey key) noexcept {
	auto mapping = FindMapping(jumpKey);
	if (!mapping)
		return;

	defaultContextMappings->UnmapKey(jump, jumpKey);
	FEnhancedActionKeyMapping* keyData = &defaultContextMappings->MapKey(jump, key);

	RebuildContextMappings();
	jumpKey = key;
}
void APrimaryPlayerController::SetShootKey(FKey key) noexcept {
	auto mapping = FindMapping(shootKey);
	if (!mapping)
		return;

	defaultContextMappings->UnmapKey(shoot, shootKey);
	FEnhancedActionKeyMapping* keyData = &defaultContextMappings->MapKey(shoot, key);

	RebuildContextMappings();
	shootKey = key;
}
void APrimaryPlayerController::SetWeaponSwitchUpKey(FKey key) noexcept {
	auto mapping = FindMapping(weaponSwitchUpKey);
	if (!mapping)
		return;

	defaultContextMappings->UnmapKey(switchNextWeapon, weaponSwitchUpKey);
	FEnhancedActionKeyMapping* keyData = &defaultContextMappings->MapKey(switchNextWeapon, key);

	RebuildContextMappings();
	weaponSwitchUpKey = key;
}
void APrimaryPlayerController::SetWeaponSwitchDownKey(FKey key) noexcept {
	auto mapping = FindMapping(weaponSwitchDownKey);
	if (!mapping)
		return;

	defaultContextMappings->UnmapKey(switchPreviousWeapon, weaponSwitchDownKey);
	FEnhancedActionKeyMapping* keyData = &defaultContextMappings->MapKey(switchPreviousWeapon, key);

	RebuildContextMappings();
	weaponSwitchDownKey = key;
}
void APrimaryPlayerController::SetReloadKey(FKey key) noexcept {
	auto mapping = FindMapping(reloadKey);
	if (!mapping)
		return;

	defaultContextMappings->UnmapKey(reload, reloadKey);
	FEnhancedActionKeyMapping* keyData = &defaultContextMappings->MapKey(reload, key);

	RebuildContextMappings();
	reloadKey = key;
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
FEnhancedActionKeyMapping* APrimaryPlayerController::FindMapping(const FKey& key) const noexcept {
	TArray<FEnhancedActionKeyMapping> allMappings = defaultContextMappings->GetMappings();
	if (allMappings.IsEmpty())
		return nullptr;
	//Copy
	for (auto& mapping : allMappings) {
		if (mapping.Key == key)
			return &mapping;
	}

	return nullptr;
}
TArray<const FEnhancedActionKeyMapping*> APrimaryPlayerController::FindMappings(const TObjectPtr<UInputAction>& action) const noexcept {
	const TArray<FEnhancedActionKeyMapping>* allMappings = &defaultContextMappings->GetMappings();
	if (allMappings && allMappings->IsEmpty())
		return {};

	TArray<const FEnhancedActionKeyMapping*> mappings;
	for (auto& mapping : *allMappings) {
		if (mapping.Action == action)
			mappings.Add(&mapping);
	}

	return mappings;
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
