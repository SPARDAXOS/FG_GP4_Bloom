#include "LevelSelectMenuWidget.h"
#include "CustomButton.h"
#include "Components/Button.h"

#include "GP4Testing/Systems/PrimaryGameMode.h"
#include "GP4Testing/Systems/PrimaryHUD.h"



void ULevelSelectMenuWidget::NativeOnInitialized() {
	UMenuWidgetBase::NativeOnInitialized();

	startButton->button->OnClicked.AddDynamic(this, &ULevelSelectMenuWidget::StartButtonClicked);
	returnButton->button->OnClicked.AddDynamic(this, &ULevelSelectMenuWidget::ReturnButtonClicked);

}


void ULevelSelectMenuWidget::StartButtonClicked() {
	//?? StartGame(LevelData)
	primaryGameModeRef->StartGame();
}
void ULevelSelectMenuWidget::ReturnButtonClicked() {
	primaryHUDRef->SetMenuState(MenuState::MAIN_MENU);
}
