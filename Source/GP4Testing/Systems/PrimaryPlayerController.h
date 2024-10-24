#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PrimaryPlayerController.generated.h"



class UEnhancedInputLocalPlayerSubsystem;
class UEnhancedInputComponent;
class UInputMappingContext;
class UInputAction;
class APrimaryPlayer;
struct FInputActionValue;
struct FEnhancedActionKeyMapping;

class UInputModifierSwizzleAxis;
class UInputModifierNegate;


enum class ControllerInputMode : uint8 {
	NONE = 0,
	GAMEPLAY,
	MENU,
	PAUSED
};


UCLASS(Abstract)
class GP4TESTING_API APrimaryPlayerController : public APlayerController {
	GENERATED_BODY()
	

private:
	enum class GameInputMode : uint8 {
		NONE = 0,
		UI_ONLY,
		GAME_ONLY,
		GAME_AND_UI
	};

public:
	virtual void BeginPlay() override;

public:
	void SetControllerInputMode(ControllerInputMode mode) noexcept;

public:
	void SetForwardKey(FKey key) noexcept;
	void SetBackwardKey(FKey key) noexcept;
	void SetRightKey(FKey key) noexcept;
	void SetLeftKey(FKey key) noexcept;
	void SetDashKey(FKey key) noexcept;
	void SetSlideKey(FKey key) noexcept;
	void SetJumpKey(FKey key) noexcept;
	void SetShootKey(FKey key) noexcept;
	void SetWeaponSwitchUpKey(FKey key) noexcept;
	void SetWeaponSwitchDownKey(FKey key) noexcept;
	void SetReloadKey(FKey key) noexcept;

	FKey GetForwardKey() const noexcept { return forwardKey; }
	FKey GetBackwardKey() const noexcept { return backwardKey; }
	FKey GetRightKey() const noexcept { return rightKey; }
	FKey GetLeftKey() const noexcept { return leftKey; }
	FKey GetDashKey() const noexcept { return dashKey; }
	FKey GetSlideKey() const noexcept { return slideKey; }
	FKey GetJumpKey() const noexcept { return jumpKey; }
	FKey GetShootKey() const noexcept { return shootKey; }
	FKey GetWeaponSwitchUpKey() const noexcept { return weaponSwitchUpKey; }
	FKey GetWeaponSwitchDownKey() const noexcept { return weaponSwitchDownKey; }
	FKey GetReloadKey() const noexcept { return reloadKey; }

protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

protected:
	void HandleMovement(const FInputActionValue& value);
	void HandleLook(const FInputActionValue& value);
	void HandleJump();
	void HandleDash();
	void HandleSlide(const FInputActionValue& value);
	void HandleShoot(const FInputActionValue& value);
	void HandlePause();
	void HandleReload();
	void HandleSwitchNextWeapon();
	void HandleSwitchPreviousWeapon();
	void HandleWeaponSlot1();
	void HandleWeaponSlot2();
	void HandleWeaponSlot3();


private:
	void SetupInputMappingContext() const noexcept;
	void SetupInputActions() noexcept;

private:
	void SetupMovementModifiers() noexcept;
	void CacheKeybindings() noexcept;
	void RebuildContextMappings() noexcept;

private:
	void SetupPausedInputMode() noexcept;
	void SetupGameplayInputMode() noexcept;
	void SetupMenuInputMode() noexcept;
	void SetupOffInputMode() noexcept;
	void SetPlayerInputState(bool state) noexcept;
	void SetCursorState(bool state) noexcept;
	void SetGameInputMode(GameInputMode mode) noexcept;
	FEnhancedActionKeyMapping* FindMapping(const FKey& key) const noexcept;
	TArray<const FEnhancedActionKeyMapping*> FindMappings(const TObjectPtr<UInputAction>& action) const noexcept;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> defaultContextMappings = nullptr;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions | Movement", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<UInputAction> movement = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions | Movement", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<UInputAction> look = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions | Movement", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<UInputAction> jump = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions | Movement", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<UInputAction> dash = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions | Movement", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<UInputAction> slide = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions | Combat", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<UInputAction> shoot = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions | Options", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<UInputAction> pauseToggle = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions | Combat", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<UInputAction> reload = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions | Combat", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<UInputAction> switchNextWeapon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions | Combat", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<UInputAction> switchPreviousWeapon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions | Combat", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<UInputAction> weaponSlot1 = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions | Combat", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<UInputAction> weaponSlot2 = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions | Combat", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<UInputAction> weaponSlot3 = nullptr;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keybindings", meta = (AllowPrivateAccess = "true"));
	FKey forwardKey;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keybindings", meta = (AllowPrivateAccess = "true"));
	FKey backwardKey;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keybindings", meta = (AllowPrivateAccess = "true"));
	FKey rightKey;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keybindings", meta = (AllowPrivateAccess = "true"));
	FKey leftKey;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keybindings", meta = (AllowPrivateAccess = "true"));
	FKey dashKey;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keybindings", meta = (AllowPrivateAccess = "true"));
	FKey slideKey;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keybindings", meta = (AllowPrivateAccess = "true"));
	FKey jumpKey;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keybindings", meta = (AllowPrivateAccess = "true"));
	FKey shootKey;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keybindings", meta = (AllowPrivateAccess = "true"));
	FKey weaponSwitchUpKey;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keybindings", meta = (AllowPrivateAccess = "true"));
	FKey weaponSwitchDownKey;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keybindings", meta = (AllowPrivateAccess = "true"));
	FKey reloadKey;

private:
	TObjectPtr<UInputModifierSwizzleAxis> forwardMovementSwizzler;

	TObjectPtr<UInputModifierNegate> leftMovementNegate;
	TObjectPtr<UInputModifierSwizzleAxis> backwardMovementSwizzler;
	TObjectPtr<UInputModifierNegate> backwardMovementNegate;

private:
	UEnhancedInputLocalPlayerSubsystem* inputSubsystemRef = nullptr;
	UEnhancedInputComponent* enhancedInputComponentRef = nullptr;
	APrimaryPlayer* primaryPlayerRef = nullptr;

private:
	ControllerInputMode currentControllerInputMode;
};
