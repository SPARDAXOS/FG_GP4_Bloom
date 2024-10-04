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
	void SetControllerInputMode(ControllerInputMode mode) noexcept;

protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

protected:
	void HandleMovement(const FInputActionValue& value);
	void HandleLook(const FInputActionValue& value);
	void HandleJump();
	void HandleShoot(const FInputActionValue& value);
	void HandlePause();

private:
	void SetupInputMappingContext() const noexcept;
	void SetupInputActions() noexcept;

private:
	void SetupPausedInputMode() noexcept;
	void SetupGameplayInputMode() noexcept;
	void SetupMenuInputMode() noexcept;
	void SetupOffInputMode() noexcept;
	void SetPlayerInputState(bool state) noexcept;
	void SetCursorState(bool state) noexcept;
	void SetGameInputMode(GameInputMode mode) noexcept;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions | Combat", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<UInputAction> shoot = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions | Options", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<UInputAction> pauseToggle = nullptr;

private:
	UEnhancedInputLocalPlayerSubsystem* inputSubsystemRef = nullptr;
	UEnhancedInputComponent* enhancedInputComponentRef = nullptr;
	APrimaryPlayer* primaryPlayerRef = nullptr;

private:
	ControllerInputMode currentControllerInputMode;
};
