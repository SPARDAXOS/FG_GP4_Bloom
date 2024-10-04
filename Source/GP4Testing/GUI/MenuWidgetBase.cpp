#include "MenuWidgetBase.h"

#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "CustomButton.h"




void UMenuWidgetBase::SetVisibilityState(ESlateVisibility state) noexcept {
	visible = state;
	SetVisibility(visible);
}
void UMenuWidgetBase::SetWidgetOpacity(float value) noexcept {
	SetRenderOpacity(value);
}



