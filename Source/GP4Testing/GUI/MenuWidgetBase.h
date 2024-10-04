#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "GP4Testing/Systems/Entity.h"

#include "MenuWidgetBase.generated.h"



class APrimaryGameMode;
class APrimaryHUD;

enum class MenuState : uint8;

UCLASS(Abstract)
class GP4TESTING_API UMenuWidgetBase : public UUserWidget, public Entity {
	GENERATED_BODY()

public:
	virtual void SetVisibilityState(ESlateVisibility state) noexcept;
	virtual void SetWidgetOpacity(float value) noexcept;

public:
	/// <summary>
	/// Internal Usage Only!
	/// </summary>
	inline void SetMenuState(MenuState state) noexcept { menuStateType = state; }
	inline void SetPrimaryGameModeReference(APrimaryGameMode& gameMode) noexcept { primaryGameModeRef = &gameMode; }
	inline void SetPrimaryHUDReference(APrimaryHUD& hud) noexcept { primaryHUDRef = &hud; }

	inline MenuState GetMenuStateType() const noexcept { return menuStateType; }
	inline ESlateVisibility GetVisibility() const noexcept { return visible; }

protected:
	MenuState menuStateType;
	ESlateVisibility visible;

protected:
	APrimaryGameMode* primaryGameModeRef = nullptr;
	APrimaryHUD* primaryHUDRef = nullptr;
};