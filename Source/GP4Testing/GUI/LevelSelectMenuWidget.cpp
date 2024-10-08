#include "LevelSelectMenuWidget.h"
#include "CustomButton.h"
#include "Components/Button.h"
#include "Components/TileView.h"
#include "Components/Image.h"

#include "GP4Testing/Systems/PrimaryGameMode.h"
#include "GP4Testing/Systems/PrimaryHUD.h"
#include "GP4Testing/GUI/LevelSelectEntry.h"

#include "GP4Testing/Utility/Debugging.h"



void ULevelSelectMenuWidget::NativeOnInitialized() {
	UMenuWidgetBase::NativeOnInitialized();

	startButton->button->OnClicked.AddDynamic(this, &ULevelSelectMenuWidget::StartButtonClicked);
	returnButton->button->OnClicked.AddDynamic(this, &ULevelSelectMenuWidget::ReturnButtonClicked);

	CreateLevelSelectEntries();
}


void ULevelSelectMenuWidget::StartButtonClicked() {
	//?? StartGame(LevelData)
	primaryGameModeRef->StartGame();
}
void ULevelSelectMenuWidget::ReturnButtonClicked() {
	primaryHUDRef->SetMenuState(MenuState::MAIN_MENU);

}


void ULevelSelectMenuWidget::CreateLevelSelectEntries() noexcept {
	if (!levelSelectEntryClass) {
		Debugging::CustomError("Failed to create level select entries due to levelSelectEntryClass being invalid!");
		return;
	}

	for (auto& entry : levelEntriesSpecs) {
		ULevelSelectEntry* newItem = CreateWidget<ULevelSelectEntry>(GetWorld(), levelSelectEntryClass);
		newItem->SetLevelSelectMenuReference(*this);
		newItem->SetTargetLevelSelectEntrySpec(&entry);
		tileView->AddItem(newItem);
		createdLevelEntries.Add(newItem);
		newItem->SetSplashImage(*entry.splash);

	}
}