#include "PrimaryPlayerHUD.h"

#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "CustomButton.h"


void UPrimaryPlayerHUD::NativeOnInitialized() {
	UUserWidget::NativeOnInitialized();


}


//Dont touch those unless you have a good reason to.
void UPrimaryPlayerHUD::SetVisibilityState(ESlateVisibility state) noexcept {
	visible = state;
	SetVisibility(visible);
}
void UPrimaryPlayerHUD::SetWidgetOpacity(float value) noexcept {
	SetRenderOpacity(value);
}



