

#include "CustomButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "GP4Testing/Utility/Debugging.h"



void UCustomButton::NativeOnInitialized() {
	UUserWidget::NativeOnInitialized();

	button->OnClicked.AddDynamic(this, &UCustomButton::ButtonClicked);
}
void UCustomButton::ButtonClicked() {
	Debugging::CustomWarning("Custom button has no callback bound to it!");
}