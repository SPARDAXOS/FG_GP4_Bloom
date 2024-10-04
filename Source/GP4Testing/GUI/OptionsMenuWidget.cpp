#include "OptionsMenuWidget.h"
#include "CustomButton.h"
#include "Components/Button.h"

#include "GP4Testing/Systems/PrimaryGameMode.h"
#include "GP4Testing/Systems/PrimaryHUD.h"



void UOptionsMenuWidget::NativeOnInitialized() {
	UMenuWidgetBase::NativeOnInitialized();

	returnButton->button->OnClicked.AddDynamic(this, &UOptionsMenuWidget::ReturnButtonClicked);
}


void UOptionsMenuWidget::ReturnButtonClicked() {
	primaryHUDRef->SetMenuState(primaryHUDRef->GetPreviousActiveMenu()->GetMenuStateType());
}
