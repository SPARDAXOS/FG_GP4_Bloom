#include "OptionsMenuWidget.h"
#include "CustomButton.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "Components/CheckBox.h"
#include "Components/InputKeySelector.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/GameUserSettings.h"

#include "GP4Testing/Systems/PrimaryGameMode.h"
#include "GP4Testing/Systems/PrimaryHUD.h"



void UOptionsMenuWidget::NativeOnInitialized() {
	UMenuWidgetBase::NativeOnInitialized();

	returnButton->button->OnClicked.AddDynamic(this, &UOptionsMenuWidget::ReturnButtonClicked);
}
void UOptionsMenuWidget::SetVisibilityState(ESlateVisibility state) noexcept {
	UMenuWidgetBase::SetVisibilityState(state);
	if (state == ESlateVisibility::Visible) {
		//DetectCurrentSettings();
		SetActiveTab(OptionsMenuTab::DISPLAY);
		//applySettingsButton->SetIsEnabled(false);
		SetConfirmationPopupState(false);
	}
}
void UOptionsMenuWidget::SetWidgetOpacity(float opacity) noexcept {
	UMenuWidgetBase::SetWidgetOpacity(opacity);
	backgroundDynamicMaterial->SetScalarParameterValue("Time", opacity);
}


void UOptionsMenuWidget::SetBackgroundType(OptionsMenuBackgroundType type) noexcept {

}
void UOptionsMenuWidget::SetActiveTab(OptionsMenuTab tab) noexcept {

}
void UOptionsMenuWidget::SetConfirmationPopupState(bool state) noexcept {

}


//Settings Detection
void UOptionsMenuWidget::DetectCurrentSettings() noexcept {

}
void UOptionsMenuWidget::DetectCurrentResolution() noexcept {

}
void UOptionsMenuWidget::DetectCurrentWindowMode() noexcept {

}
void UOptionsMenuWidget::DetectCurrentResolutionScale() noexcept {

}
void UOptionsMenuWidget::DetectCurrentDynamicResolution() noexcept {

}
void UOptionsMenuWidget::DetectCurrentVSync() noexcept {

}
void UOptionsMenuWidget::DetectCurrentFramerateLimit() noexcept {

}
void UOptionsMenuWidget::DetectCurrentTextureQuality() noexcept {

}
void UOptionsMenuWidget::DetectCurrentAntiAliasingMode() noexcept {

}
void UOptionsMenuWidget::DetectCurrentAntiAliasingQuality() noexcept {

}
void UOptionsMenuWidget::DetectCurrentShadingQuality() noexcept {

}
void UOptionsMenuWidget::DetectCurrentGlobalIlluminationQuality() noexcept {

}
void UOptionsMenuWidget::DetectCurrentPostProcessQuality() noexcept {

}
void UOptionsMenuWidget::DetectCurrentReflectionQuality() noexcept {

}
void UOptionsMenuWidget::DetectCurrentShadowQuality() noexcept {

}
void UOptionsMenuWidget::DetectCurrentVisualEffectQuality() noexcept {

}
void UOptionsMenuWidget::DetectCurrentViewDistanceQuality() noexcept {

}
void UOptionsMenuWidget::DetectCurrentFoliageQuality() noexcept {

}
void UOptionsMenuWidget::DetectCurrentAudioQuality() noexcept {

}
void UOptionsMenuWidget::DetectCurrentAudioLevels() noexcept {

}
void UOptionsMenuWidget::DetectCurrentKeybindings() noexcept {

}
void UOptionsMenuWidget::DetectCurrentSensitivitySettings() noexcept {

}


//Initial Setup
void UOptionsMenuWidget::SetupInitialResolutionSettings() noexcept {

}
void UOptionsMenuWidget::SetupInitialWindowModeSettings() noexcept {

}
void UOptionsMenuWidget::SetupInitialFramerateLimitSettings() noexcept {

}
void UOptionsMenuWidget::SetupInitialTextureQualitySettings() noexcept {

}
void UOptionsMenuWidget::SetupInitialAntiAliasingModeSettings() noexcept {

}
void UOptionsMenuWidget::SetupInitialAntiAliasingQualitySettings() noexcept {

}
void UOptionsMenuWidget::SetupInitialShadingQualitySettings() noexcept {

}
void UOptionsMenuWidget::SetupInitialGlobalIlluminationQualitySettings() noexcept {

}
void UOptionsMenuWidget::SetupInitialPostProcessingQualitySettings() noexcept {

}
void UOptionsMenuWidget::SetupInitialReflectionQualitySettings() noexcept {

}
void UOptionsMenuWidget::SetupInitialShadowQualitySettings() noexcept {

}
void UOptionsMenuWidget::SetupInitialVisualEffectQualitySettings() noexcept {

}
void UOptionsMenuWidget::SetupInitialViewDistanceQualitySettings() noexcept {

}
void UOptionsMenuWidget::SetupInitialFoliageQualitySettings() noexcept {

}
void UOptionsMenuWidget::SetupInitialAudioQualitySettings() noexcept {

}


//General Callbacks
void UOptionsMenuWidget::ReturnButtonClicked() {
	primaryHUDRef->SetMenuState(primaryHUDRef->GetPreviousActiveMenu()->GetMenuStateType());
}
void UOptionsMenuWidget::PopupConfirmButtonClicked() {

}
void UOptionsMenuWidget::PopupRevertButtonClicked() {

}
void UOptionsMenuWidget::DisplayTabButtonClicked() {

}
void UOptionsMenuWidget::GraphicsTabButtonClicked() {

}
void UOptionsMenuWidget::AudioTabButtonClicked() {

}
void UOptionsMenuWidget::InputTabButtonClicked() {

}


//Display Tab Callbacks
void UOptionsMenuWidget::ApplySettingsButtonClicked() {

}
void UOptionsMenuWidget::UpdateResolutionSelector(FString SelectedItem, ESelectInfo::Type SelectionType) {

}
void UOptionsMenuWidget::UpdateWindowModeSelector(FString SelectedItem, ESelectInfo::Type SelectionType) {

}
void UOptionsMenuWidget::UpdateResolutionScaleSlider(float value) {

}
void UOptionsMenuWidget::UpdateDynamicResolutionCheckbox(bool bIsChecked) {

}
void UOptionsMenuWidget::UpdateVSyncCheckbox(bool bIsChecked) {

}
void UOptionsMenuWidget::UpdateFramerateLimitSelector(FString SelectedItem, ESelectInfo::Type SelectionType) {

}


//Graphics Tab Callbacks
void UOptionsMenuWidget::UpdateTextureQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType) {

}
void UOptionsMenuWidget::UpdateAntiAliasingModeSelector(FString SelectedItem, ESelectInfo::Type SelectionType) {

}
void UOptionsMenuWidget::UpdateAntiAliasingQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType) {

}
void UOptionsMenuWidget::UpdateShadingQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType) {

}
void UOptionsMenuWidget::UpdateGlobalIlluminationQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType) {

}
void UOptionsMenuWidget::UpdatePostProcessingQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType) {

}
void UOptionsMenuWidget::UpdateReflectionQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType) {

}
void UOptionsMenuWidget::UpdateShadowQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType) {

}
void UOptionsMenuWidget::UpdateVisualEffectQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType) {

}
void UOptionsMenuWidget::UpdateViewDistanceQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType) {

}
void UOptionsMenuWidget::UpdateFoliageQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType) {

}
void UOptionsMenuWidget::UpdateAudioQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType) {

}


//Audio Tab Callbacks
void UOptionsMenuWidget::UpdateMasterVolumeSlider(float value) {

}


//Input Tab Callbacks
void UOptionsMenuWidget::UpdateForwardKeySelector(FInputChord SelectedKey) {

}
void UOptionsMenuWidget::UpdateBackwardKeySelector(FInputChord SelectedKey) {

}
void UOptionsMenuWidget::UpdateRightKeySelector(FInputChord SelectedKey) {

}
void UOptionsMenuWidget::UpdateLeftKeySelector(FInputChord SelectedKey) {

}
void UOptionsMenuWidget::UpdateDashKeySelector(FInputChord SelectedKey) {

}
void UOptionsMenuWidget::UpdateSlideKeySelector(FInputChord SelectedKey) {

}
void UOptionsMenuWidget::UpdateJumpKeySelector(FInputChord SelectedKey) {

}
void UOptionsMenuWidget::UpdateShootKeySelector(FInputChord SelectedKey) {

}
void UOptionsMenuWidget::UpdateWeaponSwitchUpKeySelector(FInputChord SelectedKey) {

}
void UOptionsMenuWidget::UpdateWeaponSwitchDownKeySelector(FInputChord SelectedKey) {

}
void UOptionsMenuWidget::UpdateReloadKeySelector(FInputChord SelectedKey) {

}
void UOptionsMenuWidget::UpdateMouseSensitivitySlider(float value) {

}