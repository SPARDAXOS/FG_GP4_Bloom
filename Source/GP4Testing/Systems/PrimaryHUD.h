#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "Entity.h"

#include "PrimaryHUD.generated.h"



class APrimaryGameMode;
class APrimaryPlayerController;
class APrimaryPlayer;

class UMenuWidgetBase;
class UMainMenuWidget;
class UOptionsMenuWidget;
class ULevelSelectMenuWidget;
class UPauseMenuWidget;
class UWinMenuWidget;
class ULoseMenuWidget;



UENUM(BlueprintType)
enum class MenuState : uint8 {
	NONE = 0,
	MAIN_MENU,
	OPTIONS_MENU,
	LEVEL_SELECT_MENU,
	PAUSE_MENU,
	WIN_MENU,
	LOSE_MENU
};



UCLASS(Abstract)
class GP4TESTING_API APrimaryHUD : public AHUD, public Entity {
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void Start() override;
	virtual void Update(float deltaTime) override;

public:
	bool SetMenuState(MenuState state) noexcept;
	void ClearViewport() noexcept;

public:
	inline UMenuWidgetBase* GetCurrentActiveMenu() noexcept { return currentActiveMenu; }
	inline UMenuWidgetBase* GetPreviousActiveMenu() noexcept { return previousActiveMenu; }
	inline MenuState GetCurrentActiveMenuState() const noexcept { return currentMenuState; }

public:
	void SetupDependencies(APrimaryGameMode& gameMode, APrimaryPlayer& player, APrimaryPlayerController& controller) noexcept;

private:
	void UpdateActiveMenu(float deltaTime) noexcept;

private:
	void CreateAllMenus() noexcept;
	void InitializeAllMenus() noexcept;
	void StartAllMenus() noexcept;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PrimaryHUD|Assets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMainMenuWidget> mainMenuWidgetClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PrimaryHUD|Assets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UOptionsMenuWidget> optionsMenuWidgetClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PrimaryHUD|Assets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ULevelSelectMenuWidget> levelSelectMenuWidgetClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PrimaryHUD|Assets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UPauseMenuWidget> pauseMenuWidgetClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PrimaryHUD|Assets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UWinMenuWidget> winMenuWidgetClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PrimaryHUD|Assets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ULoseMenuWidget> loseMenuWidgetClass = nullptr;

private:
	UPROPERTY(VisibleAnywhere, Category = "PrimaryHUD|Debugging", meta = (AllowPrivateAccess = "true"))
	MenuState currentMenuState = MenuState::NONE;

private:
	TObjectPtr<UMainMenuWidget> mainMenuRef = nullptr;
	TObjectPtr<UOptionsMenuWidget> optionsMenuRef = nullptr;
	TObjectPtr<ULevelSelectMenuWidget> levelSelectMenuRef = nullptr;
	TObjectPtr<UPauseMenuWidget> pauseMenuRef = nullptr;
	TObjectPtr<UWinMenuWidget> winMenuRef = nullptr;
	TObjectPtr<ULoseMenuWidget> loseMenuRef = nullptr;

private:
	APrimaryGameMode* primaryGameModeRef = nullptr;
	APrimaryPlayerController* primaryPlayerControllerRef = nullptr;
	APrimaryPlayer* primaryPlayerRef = nullptr;

private:
	UMenuWidgetBase* currentActiveMenu = nullptr;
	UMenuWidgetBase* previousActiveMenu = nullptr;

};
