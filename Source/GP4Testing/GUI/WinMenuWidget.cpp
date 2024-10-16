#include "WinMenuWidget.h"
#include "CustomButton.h"
#include "Components/Button.h"

#include "GP4Testing/Systems/PrimaryGameMode.h"
#include "GP4Testing/Systems/PrimaryHUD.h"



void UWinMenuWidget::NativeOnInitialized() {
	UMenuWidgetBase::NativeOnInitialized();

	nextButton->button->OnClicked.AddDynamic(this, &UWinMenuWidget::NextButtonClicked);
	quitButton->button->OnClicked.AddDynamic(this, &UWinMenuWidget::QuitButtonClicked);
}


void UWinMenuWidget::NextButtonClicked() {
	primaryGameModeRef->ProgressGame();
}
void UWinMenuWidget::QuitButtonClicked() {
	primaryGameModeRef->EndGame();
}