#include "MainMenuWidget.h"
#include "CustomButton.h"
#include "Components/Button.h"

#include "GP4Testing/Systems/PrimaryGameMode.h"
#include "GP4Testing/Systems/PrimaryHUD.h"
#include "GP4Testing/GUI/OptionsMenuWidget.h"



void UMainMenuWidget::NativeOnInitialized() {
	UMenuWidgetBase::NativeOnInitialized();

	startButton->button->OnClicked.AddDynamic(this, &UMainMenuWidget::StartButtonClicked);
	optionsButton->button->OnClicked.AddDynamic(this, &UMainMenuWidget::OptionsButtonClicked);
	quitButton->button->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitButtonClicked);
}


void UMainMenuWidget::StartButtonClicked() {
	primaryHUDRef->SetMenuState(MenuState::LEVEL_SELECT_MENU);
}
void UMainMenuWidget::OptionsButtonClicked() {
	primaryHUDRef->GetOptionsMenu()->SetBackgroundType(OptionsMenuBackgroundType::STATIC);
	primaryHUDRef->SetMenuState(MenuState::OPTIONS_MENU);
}
void UMainMenuWidget::QuitButtonClicked() {
	primaryGameModeRef->QuitGame();
}