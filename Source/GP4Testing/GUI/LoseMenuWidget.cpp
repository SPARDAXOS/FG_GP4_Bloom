#include "LoseMenuWidget.h"
#include "CustomButton.h"
#include "Components/Button.h"

#include "GP4Testing/Systems/PrimaryGameMode.h"
#include "GP4Testing/Systems/PrimaryHUD.h"



void ULoseMenuWidget::NativeOnInitialized() {
	UMenuWidgetBase::NativeOnInitialized();

	restartButton->button->OnClicked.AddDynamic(this, &ULoseMenuWidget::RestartButtonClicked);
	quitButton->button->OnClicked.AddDynamic(this, &ULoseMenuWidget::QuitButtonClicked);
}


void ULoseMenuWidget::RestartButtonClicked() {
	primaryGameModeRef->RestartGame();
}
void ULoseMenuWidget::QuitButtonClicked() {
	primaryGameModeRef->EndGame();
}