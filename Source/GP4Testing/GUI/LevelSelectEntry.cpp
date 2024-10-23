#include "LevelSelectEntry.h"

#include "GP4Testing/GUI/CustomButton.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"

#include "GP4Testing/Systems/PrimaryGameMode.h"
#include "GP4Testing/Systems/PrimaryHUD.h"
#include "GP4Testing/GUI/LevelSelectMenuWidget.h"
#include "GP4Testing/DataAssets/LevelSelectEntrySpec.h"
#include "GP4Testing/Utility/Debugging.h"



void ULevelSelectEntry::NativeOnListItemObjectSet(UObject* ListItemObject) {
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	ULevelSelectEntrySpec* spec = Cast<ULevelSelectEntrySpec>(ListItemObject);
	if (!spec) {
		Debugging::CustomError("TileView element does not inherit from ULevelSelectEntrySpec");
		return;
	}

	SetTargetLevelSelectEntrySpec(spec);
	SetNameText(spec->name);

	if (!spec->splash)
		Debugging::CustomWarning("ULevelSelectEntrySpec splash is invalid!");
	else
		SetSplashImage(*spec->splash); 

	if (!spec->managingMenu)
		Debugging::CustomWarning("ULevelSelectEntrySpec managingMenu is invalid!");
	else
		SetLevelSelectMenuReference(*spec->managingMenu);

	selectionFrameImage->SetVisibility(ESlateVisibility::Collapsed);
}
void ULevelSelectEntry::NativeOnItemSelectionChanged(bool selected) {
	IUserObjectListEntry::NativeOnItemSelectionChanged(selected);

	if (selected && levelSelectMenuWidgetRef && targetSpec) {
		levelSelectMenuWidgetRef->SetSelectedLevelEntrySpec(targetSpec);
		selectionFrameImage->SetVisibility(ESlateVisibility::Visible);
	}
	else if (!selected)
		selectionFrameImage->SetVisibility(ESlateVisibility::Collapsed);
}


void ULevelSelectEntry::SetSplashImage(UMaterialInterface& splash) noexcept {
	splashImage->SetBrushFromMaterial(&splash);
}
void ULevelSelectEntry::SetNameText(const FName& name) noexcept {
	nameText->SetText(FText::FromName(name));
}
