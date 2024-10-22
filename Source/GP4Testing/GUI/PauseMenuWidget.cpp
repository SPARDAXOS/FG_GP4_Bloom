#include "PauseMenuWidget.h"
#include "CustomButton.h"
#include "Components/Button.h"

#include "GP4Testing/Systems/PrimaryGameMode.h"
#include "GP4Testing/Systems/PrimaryHUD.h"
#include "GP4Testing/GUI/OptionsMenuWidget.h"



void UPauseMenuWidget::NativeOnInitialized() {
	UMenuWidgetBase::NativeOnInitialized();

	continueButton->button->OnClicked.AddDynamic(this, &UPauseMenuWidget::ContinueButtonClicked);
	optionsButton->button->OnClicked.AddDynamic(this, &UPauseMenuWidget::OptionsButtonClicked);
	quitButton->button->OnClicked.AddDynamic(this, &UPauseMenuWidget::QuitButtonClicked);
}


void UPauseMenuWidget::ContinueButtonClicked() {
	primaryGameModeRef->UnpauseGame();
}
void UPauseMenuWidget::OptionsButtonClicked() {
	primaryHUDRef->GetOptionsMenu()->SetBackgroundType(OptionsMenuBackgroundType::STATIC);
	primaryHUDRef->SetMenuState(MenuState::OPTIONS_MENU);
}
void UPauseMenuWidget::QuitButtonClicked() {
	primaryGameModeRef->EndGame();
}