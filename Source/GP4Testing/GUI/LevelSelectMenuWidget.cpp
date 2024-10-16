#include "LevelSelectMenuWidget.h"
#include "CustomButton.h"
#include "Components/Button.h"
#include "Components/TileView.h"
#include "Components/Image.h"

#include "GP4Testing/Systems/PrimaryGameMode.h"
#include "GP4Testing/Systems/PrimaryHUD.h"
#include "GP4Testing/GUI/LevelSelectEntry.h"
#include "GP4Testing/DataAssets/LevelSelectEntrySpec.h"

#include "GP4Testing/Utility/Debugging.h"



void ULevelSelectMenuWidget::NativeOnInitialized() {
	UMenuWidgetBase::NativeOnInitialized();

	startButton->button->OnClicked.AddDynamic(this, &ULevelSelectMenuWidget::StartButtonClicked);
	returnButton->button->OnClicked.AddDynamic(this, &ULevelSelectMenuWidget::ReturnButtonClicked);

	CreateLevelSelectEntries();
	startButton->SetIsEnabled(false);
}
void ULevelSelectMenuWidget::SetVisibilityState(ESlateVisibility state) noexcept {
	UMenuWidgetBase::SetVisibilityState(state);

	if (state == ESlateVisibility::Visible) {
		if (selectedLevelEntrySpec)
			startButton->SetIsEnabled(true);
		else
			startButton->SetIsEnabled(false);
	}
	else if (state == ESlateVisibility::Collapsed) {
		selectedLevelEntrySpec = nullptr;
	}
}


void ULevelSelectMenuWidget::SetSelectedLevelEntrySpec(ULevelSelectEntrySpec* entry) noexcept {
	selectedLevelEntrySpec = entry; 
	if (selectedLevelEntrySpec)
		startButton->SetIsEnabled(true);
	else
		startButton->SetIsEnabled(false);
}


void ULevelSelectMenuWidget::StartButtonClicked() {
	if (selectedLevelEntrySpec) {
		primaryGameModeRef->StartGame(*selectedLevelEntrySpec);
		tileView->ClearSelection();
	}
	else
		Debugging::CustomError("Attempted to start the game with no level selected!");
}
void ULevelSelectMenuWidget::ReturnButtonClicked() {
	primaryHUDRef->SetMenuState(MenuState::MAIN_MENU);

}


void ULevelSelectMenuWidget::CreateLevelSelectEntries() noexcept {
	for (auto& entry : levelSelectEntrySpecAssets) {
		entry->managingMenu = this;
		tileView->AddItem(entry);
	}
}