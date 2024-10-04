#include "GP4Testing/Systems/PrimaryHUD.h"
#include "GP4Testing/Systems/PrimaryGameMode.h"

#include "GP4Testing/GUI/MenuWidgetBase.h"
#include "GP4Testing/GUI/MainMenuWidget.h"
#include "GP4Testing/GUI/OptionsMenuWidget.h"
#include "GP4Testing/GUI/LevelSelectMenuWidget.h"
#include "GP4Testing/GUI/PauseMenuWidget.h"



void APrimaryHUD::Init() {
	CreateAllMenus();
	InitializeAllMenus();
}
void APrimaryHUD::Start() {
	StartAllMenus();
}
void APrimaryHUD::Update(float deltaTime) {
	UpdateActiveMenu(deltaTime);
}


bool APrimaryHUD::SetMenuState(MenuState state) noexcept {
	if (currentMenuState == state)
		return false;
	if (state == MenuState::NONE)
		return false;

	currentMenuState = state;
	if (currentActiveMenu)
		currentActiveMenu->SetVisibilityState(ESlateVisibility::Collapsed);

	previousActiveMenu = currentActiveMenu;
	switch (state) {
	case MenuState::MAIN_MENU:
		currentActiveMenu = mainMenuRef;
		break;
	case MenuState::OPTIONS_MENU:
		currentActiveMenu = optionsMenuRef;
		break;
	case MenuState::LEVEL_SELECT_MENU:
		currentActiveMenu = levelSelectMenuRef;
		break;
	case MenuState::PAUSE_MENU:
		currentActiveMenu = pauseMenuRef;
		break;
	}

	currentActiveMenu->SetVisibilityState(ESlateVisibility::Visible);
	return true;
}
void APrimaryHUD::ClearViewport() noexcept {
	currentMenuState = MenuState::NONE;
	if (currentActiveMenu)
		currentActiveMenu->SetVisibilityState(ESlateVisibility::Collapsed);

	currentActiveMenu = nullptr;
	previousActiveMenu = nullptr;
}



void APrimaryHUD::SetupDependencies(APrimaryGameMode& gameMode, APrimaryPlayer& player, APrimaryPlayerController& controller) noexcept {
	primaryGameModeRef = &gameMode;
	primaryPlayerControllerRef = &controller;
	primaryPlayerRef = &player;

	mainMenuRef->SetMenuState(MenuState::MAIN_MENU);
	optionsMenuRef->SetMenuState(MenuState::OPTIONS_MENU);
	levelSelectMenuRef->SetMenuState(MenuState::LEVEL_SELECT_MENU);
	pauseMenuRef->SetMenuState(MenuState::PAUSE_MENU);

	mainMenuRef->SetPrimaryHUDReference(*this);
	optionsMenuRef->SetPrimaryHUDReference(*this);
	levelSelectMenuRef->SetPrimaryHUDReference(*this);
	pauseMenuRef->SetPrimaryHUDReference(*this);

	mainMenuRef->SetPrimaryGameModeReference(gameMode);
	optionsMenuRef->SetPrimaryGameModeReference(gameMode);
	levelSelectMenuRef->SetPrimaryGameModeReference(gameMode);
	pauseMenuRef->SetPrimaryGameModeReference(gameMode);
}
void APrimaryHUD::UpdateActiveMenu(float deltaTime) noexcept {
	if (currentActiveMenu)
		currentActiveMenu->Update(deltaTime);
}


void APrimaryHUD::CreateAllMenus() noexcept {
	checkf(mainMenuWidgetClass, TEXT("MainMenuWidgetClass is invalid!"));
	checkf(optionsMenuWidgetClass, TEXT("OptionsMenuWidgetClass is invalid!"));
	checkf(levelSelectMenuWidgetClass, TEXT("LevelSelectMenuWidgetClass is invalid!"));
	checkf(pauseMenuWidgetClass, TEXT("PauseMenuWidgetClass is invalid!"));

	mainMenuRef = CreateWidget<UMainMenuWidget>(GetWorld(), mainMenuWidgetClass);
	optionsMenuRef = CreateWidget<UOptionsMenuWidget>(GetWorld(), optionsMenuWidgetClass);
	levelSelectMenuRef = CreateWidget<ULevelSelectMenuWidget>(GetWorld(), levelSelectMenuWidgetClass);
	pauseMenuRef = CreateWidget<UPauseMenuWidget>(GetWorld(), pauseMenuWidgetClass);

	mainMenuRef->AddToViewport();
	optionsMenuRef->AddToViewport();
	levelSelectMenuRef->AddToViewport();
	pauseMenuRef->AddToViewport();

	mainMenuRef->SetVisibilityState(ESlateVisibility::Collapsed);
	optionsMenuRef->SetVisibilityState(ESlateVisibility::Collapsed);
	levelSelectMenuRef->SetVisibilityState(ESlateVisibility::Collapsed);
	pauseMenuRef->SetVisibilityState(ESlateVisibility::Collapsed);
}
void APrimaryHUD::InitializeAllMenus() noexcept {
	mainMenuRef->Init();
	optionsMenuRef->Init();
	levelSelectMenuRef->Init();
	pauseMenuRef->Init();

}
void APrimaryHUD::StartAllMenus() noexcept {
	mainMenuRef->Start();
	optionsMenuRef->Start();
	levelSelectMenuRef->Start();
	pauseMenuRef->Start();
}
