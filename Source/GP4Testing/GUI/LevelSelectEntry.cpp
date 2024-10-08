#include "LevelSelectEntry.h"

#include "GP4Testing/GUI/CustomButton.h"
#include "Components/Button.h"
#include "Components/Image.h"

#include "GP4Testing/Systems/PrimaryGameMode.h"
#include "GP4Testing/Systems/PrimaryHUD.h"
#include "GP4Testing/GUI/LevelSelectMenuWidget.h"
#include "GP4Testing/Utility/Debugging.h"



void ULevelSelectEntry::NativeOnInitialized() {
	UMenuWidgetBase::NativeOnInitialized();

	//selectButton->button->OnClicked.AddDynamic(this, &ULevelSelectEntry::SelectButtonClicked);
}
void ULevelSelectEntry::SetSplashImage(UMaterialInterface& splash) noexcept {
	splashImage->SetBrushFromMaterial(&splash);


	Debugging::PrintString("Edited splash images!");
}


void ULevelSelectEntry::SelectButtonClicked() {
	levelSelectMenuWidgetRef->SetSelectedLevelEntry(this);
}