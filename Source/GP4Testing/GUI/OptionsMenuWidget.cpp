#include "OptionsMenuWidget.h"
#include "CustomButton.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "Components/CheckBox.h"
#include "Components/InputKeySelector.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/GameUserSettings.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "AudioDevice.h"

#include "GP4Testing/Systems/PrimaryGameMode.h"
#include "GP4Testing/PlayerSystems/PlayerMovementSystem.h"
#include "GP4Testing/Systems/PrimaryPlayer.h"
#include "GP4Testing/Systems/PrimaryHUD.h"
#include "GP4Testing/Systems/PrimaryPlayerController.h"


void UOptionsMenuWidget::NativeOnInitialized() {
	UMenuWidgetBase::NativeOnInitialized();

	returnButton->button->OnClicked.AddDynamic(this, &UOptionsMenuWidget::ReturnButtonClicked);
	popupConfirmButton->button->OnClicked.AddDynamic(this, &UOptionsMenuWidget::PopupConfirmButtonClicked);
	popupRevertButton->button->OnClicked.AddDynamic(this, &UOptionsMenuWidget::PopupRevertButtonClicked);

	displayTabButton->button->OnClicked.AddDynamic(this, &UOptionsMenuWidget::DisplayTabButtonClicked);
	graphicsTabButton->button->OnClicked.AddDynamic(this, &UOptionsMenuWidget::GraphicsTabButtonClicked);
	audioTabButton->button->OnClicked.AddDynamic(this, &UOptionsMenuWidget::AudioTabButtonClicked);
	inputTabButton->button->OnClicked.AddDynamic(this, &UOptionsMenuWidget::InputTabButtonClicked);

	masterVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionsMenuWidget::UpdateMasterVolumeSlider);
	musicVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionsMenuWidget::UpdateMusicVolumeSlider);
	sfxVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionsMenuWidget::UpdateSFXVolumeSlider);

	applySettingsButton->button->OnClicked.AddDynamic(this, &UOptionsMenuWidget::ApplySettingsButtonClicked);
	resolutionSelector->OnSelectionChanged.AddDynamic(this, &UOptionsMenuWidget::UpdateResolutionSelector);
	windowModeSelector->OnSelectionChanged.AddDynamic(this, &UOptionsMenuWidget::UpdateWindowModeSelector);
	framerateLimitSelector->OnSelectionChanged.AddDynamic(this, &UOptionsMenuWidget::UpdateFramerateLimitSelector);
	resolutionScaleSlider->OnValueChanged.AddDynamic(this, &UOptionsMenuWidget::UpdateResolutionScaleSlider);
	dynamicResolutionCheckbox->OnCheckStateChanged.AddDynamic(this, &UOptionsMenuWidget::UpdateDynamicResolutionCheckbox);
	vsyncCheckbox->OnCheckStateChanged.AddDynamic(this, &UOptionsMenuWidget::UpdateVSyncCheckbox);


	textureQualitySelector->OnSelectionChanged.AddDynamic(this, &UOptionsMenuWidget::UpdateTextureQualitySelector);
	antiAliasingModeSelector->OnSelectionChanged.AddDynamic(this, &UOptionsMenuWidget::UpdateAntiAliasingModeSelector);
	antiAliasingQualitySelector->OnSelectionChanged.AddDynamic(this, &UOptionsMenuWidget::UpdateAntiAliasingQualitySelector);
	shadingQualitySelector->OnSelectionChanged.AddDynamic(this, &UOptionsMenuWidget::UpdateShadingQualitySelector);
	globalIlluminationQualitySelector->OnSelectionChanged.AddDynamic(this, &UOptionsMenuWidget::UpdateGlobalIlluminationQualitySelector);
	postProcessingQualitySelector->OnSelectionChanged.AddDynamic(this, &UOptionsMenuWidget::UpdatePostProcessingQualitySelector);
	reflectionQualitySelector->OnSelectionChanged.AddDynamic(this, &UOptionsMenuWidget::UpdateReflectionQualitySelector);
	shadowQualitySelector->OnSelectionChanged.AddDynamic(this, &UOptionsMenuWidget::UpdateShadowQualitySelector);
	visualEffectQualitySelector->OnSelectionChanged.AddDynamic(this, &UOptionsMenuWidget::UpdateVisualEffectQualitySelector);
	viewDistanceQualitySelector->OnSelectionChanged.AddDynamic(this, &UOptionsMenuWidget::UpdateViewDistanceQualitySelector);
	foliageQualitySelector->OnSelectionChanged.AddDynamic(this, &UOptionsMenuWidget::UpdateFoliageQualitySelector);
	audioQualitySelector->OnSelectionChanged.AddDynamic(this, &UOptionsMenuWidget::UpdateAudioQualitySelector);


	forwardKeySelector->OnKeySelected.AddDynamic(this, &UOptionsMenuWidget::UpdateForwardKeySelector);
	backwardKeySelector->OnKeySelected.AddDynamic(this, &UOptionsMenuWidget::UpdateBackwardKeySelector);
	rightKeySelector->OnKeySelected.AddDynamic(this, &UOptionsMenuWidget::UpdateRightKeySelector);
	leftKeySelector->OnKeySelected.AddDynamic(this, &UOptionsMenuWidget::UpdateLeftKeySelector);
	dashKeySelector->OnKeySelected.AddDynamic(this, &UOptionsMenuWidget::UpdateDashKeySelector);
	slideKeySelector->OnKeySelected.AddDynamic(this, &UOptionsMenuWidget::UpdateSlideKeySelector);
	jumpKeySelector->OnKeySelected.AddDynamic(this, &UOptionsMenuWidget::UpdateJumpKeySelector);
	shootKeySelector->OnKeySelected.AddDynamic(this, &UOptionsMenuWidget::UpdateShootKeySelector);
	weaponSwitchUpKeySelector->OnKeySelected.AddDynamic(this, &UOptionsMenuWidget::UpdateWeaponSwitchUpKeySelector);
	weaponSwitchDownKeySelector->OnKeySelected.AddDynamic(this, &UOptionsMenuWidget::UpdateWeaponSwitchDownKeySelector);
	reloadKeySelector->OnKeySelected.AddDynamic(this, &UOptionsMenuWidget::UpdateReloadKeySelector);
	mouseSensitivitySlider->OnValueChanged.AddDynamic(this, &UOptionsMenuWidget::UpdateMouseSensitivitySlider);

	gameSettings = GEngine->GetGameUserSettings();

	backgroundDynamicMaterial = dynamicBackground->GetDynamicMaterial();

	SetupInitialWindowModeSettings();
	SetupInitialResolutionSettings();
	SetupInitialFramerateLimitSettings();
	SetupInitialTextureQualitySettings();
	SetupInitialAntiAliasingModeSettings();
	SetupInitialAntiAliasingQualitySettings();
	SetupInitialShadingQualitySettings();
	SetupInitialGlobalIlluminationQualitySettings();
	SetupInitialPostProcessingQualitySettings();
	SetupInitialReflectionQualitySettings();
	SetupInitialShadowQualitySettings();
	SetupInitialVisualEffectQualitySettings();
	SetupInitialViewDistanceQualitySettings();
	SetupInitialFoliageQualitySettings();
	SetupInitialAudioQualitySettings();

	//Clears any dirt flags at the start of the application caused by auto-configuration.
	gameSettings->ApplySettings(false); 
}
void UOptionsMenuWidget::SetVisibilityState(ESlateVisibility state) noexcept {
	UMenuWidgetBase::SetVisibilityState(state);
	if (state == ESlateVisibility::Visible) {
		DetectCurrentSettings();
		SetActiveTab(OptionsMenuTab::DISPLAY);
		applySettingsButton->SetIsEnabled(false);
		SetConfirmationPopupState(false);
	}
}
void UOptionsMenuWidget::SetWidgetOpacity(float opacity) noexcept {
	UMenuWidgetBase::SetWidgetOpacity(opacity);
	backgroundDynamicMaterial->SetScalarParameterValue("Time", opacity);
}


void UOptionsMenuWidget::SetBackgroundType(OptionsMenuBackgroundType type) noexcept {
	if (currentBackgroundType == type)
		return;

	currentBackgroundType = type;
	if (currentBackgroundType == OptionsMenuBackgroundType::STATIC) {
		staticBackground->SetRenderOpacity(1.0f);
		dynamicBackground->SetRenderOpacity(0.0f);
		backgroundDynamicMaterial->SetScalarParameterValue("Time", 0.0f);
	}
	else if (currentBackgroundType == OptionsMenuBackgroundType::DYNAMIC) {
		staticBackground->SetRenderOpacity(0.0f);
		dynamicBackground->SetRenderOpacity(1.0f);
		backgroundDynamicMaterial->SetScalarParameterValue("Time", 1.0f);
	}
	else if (currentBackgroundType == OptionsMenuBackgroundType::NONE) {
		staticBackground->SetRenderOpacity(0.0f);
		dynamicBackground->SetRenderOpacity(0.0f);
		backgroundDynamicMaterial->SetScalarParameterValue("Time", 0.0f);
	}
}
void UOptionsMenuWidget::SetActiveTab(OptionsMenuTab tab) noexcept {
	if (tab == OptionsMenuTab::NONE) {
		displayTabCanvas->SetVisibility(ESlateVisibility::Collapsed);
		graphicsTabCanvas->SetVisibility(ESlateVisibility::Collapsed);
		audioTabCanvas->SetVisibility(ESlateVisibility::Collapsed);
		inputTabCanvas->SetVisibility(ESlateVisibility::Collapsed);
		applySettingsButton->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if (tab == OptionsMenuTab::DISPLAY) {
		displayTabCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		graphicsTabCanvas->SetVisibility(ESlateVisibility::Collapsed);
		audioTabCanvas->SetVisibility(ESlateVisibility::Collapsed);
		inputTabCanvas->SetVisibility(ESlateVisibility::Collapsed);
		applySettingsButton->SetVisibility(ESlateVisibility::Visible);
	}
	else if (tab == OptionsMenuTab::GRAPHICS) {
		displayTabCanvas->SetVisibility(ESlateVisibility::Collapsed);
		graphicsTabCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		audioTabCanvas->SetVisibility(ESlateVisibility::Collapsed);
		inputTabCanvas->SetVisibility(ESlateVisibility::Collapsed);
		applySettingsButton->SetVisibility(ESlateVisibility::Visible);
	}
	else if (tab == OptionsMenuTab::AUDIO) {
		displayTabCanvas->SetVisibility(ESlateVisibility::Collapsed);
		graphicsTabCanvas->SetVisibility(ESlateVisibility::Collapsed);
		audioTabCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		inputTabCanvas->SetVisibility(ESlateVisibility::Collapsed);
		applySettingsButton->SetVisibility(ESlateVisibility::Visible);
	}
	else if (tab == OptionsMenuTab::INPUT) {
		displayTabCanvas->SetVisibility(ESlateVisibility::Collapsed);
		graphicsTabCanvas->SetVisibility(ESlateVisibility::Collapsed);
		audioTabCanvas->SetVisibility(ESlateVisibility::Collapsed);
		inputTabCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		applySettingsButton->SetVisibility(ESlateVisibility::Visible);
	}
}
void UOptionsMenuWidget::SetConfirmationPopupState(bool state) noexcept {
	if (state)
		confirmationPopupCanvas->SetVisibility(ESlateVisibility::Visible);
	else
		confirmationPopupCanvas->SetVisibility(ESlateVisibility::Collapsed);
}


//Settings Detection
void UOptionsMenuWidget::DetectCurrentSettings() noexcept {

	DetectCurrentResolution();
	DetectCurrentWindowMode();
	DetectCurrentResolutionScale();
	DetectCurrentDynamicResolution();
	DetectCurrentVSync();
	DetectCurrentFramerateLimit();

	DetectCurrentTextureQuality();
	DetectCurrentAntiAliasingMode();
	DetectCurrentAntiAliasingQuality();
	DetectCurrentShadingQuality();
	DetectCurrentGlobalIlluminationQuality();

	DetectCurrentPostProcessQuality();
	DetectCurrentReflectionQuality();
	DetectCurrentShadowQuality();
	DetectCurrentVisualEffectQuality();
	DetectCurrentViewDistanceQuality();
	DetectCurrentFoliageQuality();
	DetectCurrentAudioQuality();
	DetectCurrentAudioLevels();
	DetectCurrentKeybindings();
	DetectCurrentSensitivitySettings();
}
void UOptionsMenuWidget::DetectCurrentResolution() noexcept {
	for (auto& resolution : resolutionOptions) {
		if (gameSettings->GetScreenResolution().X == resolution.resolution.X && gameSettings->GetScreenResolution().Y == resolution.resolution.Y)
			resolutionSelector->SetSelectedOption(resolution.key);
	}
}
void UOptionsMenuWidget::DetectCurrentWindowMode() noexcept {
	windowModeSelector->SetSelectedOption(LexToString(gameSettings->GetFullscreenMode()));
}
void UOptionsMenuWidget::DetectCurrentResolutionScale() noexcept {
	float value = gameSettings->GetResolutionScaleNormalized();
	if (value <= 0.0f) {
		resolutionScaleSlider->SetValue(1.0f); //If current display resolution is used then it would default the value to 0.
		resolutionScalePercentageText->SetText(FText::FromName(TEXT("100%")));
	}
	else {
		FString percentage = FString::FromInt(value * 100);
		percentage.Append("%");
		resolutionScaleSlider->SetValue(value);
		resolutionScalePercentageText->SetText(FText::FromString(percentage));
	}
}
void UOptionsMenuWidget::DetectCurrentDynamicResolution() noexcept {
	bool state = gameSettings->IsDynamicResolutionEnabled();
	if (state)
		dynamicResolutionCheckbox->SetCheckedState(ECheckBoxState::Checked);
	else
		dynamicResolutionCheckbox->SetCheckedState(ECheckBoxState::Unchecked);
}
void UOptionsMenuWidget::DetectCurrentVSync() noexcept {
	bool state = gameSettings->IsVSyncEnabled();
	if (state)
		vsyncCheckbox->SetCheckedState(ECheckBoxState::Checked);
	else
		vsyncCheckbox->SetCheckedState(ECheckBoxState::Unchecked);
}
void UOptionsMenuWidget::DetectCurrentFramerateLimit() noexcept {
	float currentFramerateLimit = gameSettings->GetFrameRateLimit();
	if (currentFramerateLimit == 30.0f)
		framerateLimitSelector->SetSelectedIndex(0);
	else if (currentFramerateLimit == 60.0f)
		framerateLimitSelector->SetSelectedIndex(1);
	else if (currentFramerateLimit == 120.0f)
		framerateLimitSelector->SetSelectedIndex(2);
	else if (currentFramerateLimit == 144.0f)
		framerateLimitSelector->SetSelectedIndex(3);
	else if (currentFramerateLimit == 0.0f)
		framerateLimitSelector->SetSelectedIndex(4);
}
void UOptionsMenuWidget::DetectCurrentTextureQuality() noexcept {
	int32 currentTextureQuality = gameSettings->GetTextureQuality();
	if (currentTextureQuality == 0)
		textureQualitySelector->SetSelectedIndex(0);
	else if (currentTextureQuality == 1)
		textureQualitySelector->SetSelectedIndex(1);
	else if (currentTextureQuality == 2)
		textureQualitySelector->SetSelectedIndex(2);
	else if (currentTextureQuality == 3)
		textureQualitySelector->SetSelectedIndex(3);
	else if (currentTextureQuality == 4)
		textureQualitySelector->SetSelectedIndex(4);
}
void UOptionsMenuWidget::DetectCurrentAntiAliasingMode() noexcept {
	int32 currentAntiAliasingMode = UKismetSystemLibrary::GetConsoleVariableIntValue("r.AntiAliasingMethod");
	if (currentAntiAliasingMode == 0)
		antiAliasingModeSelector->SetSelectedIndex(0);
	else if (currentAntiAliasingMode == 1)
		antiAliasingModeSelector->SetSelectedIndex(1);
	else if (currentAntiAliasingMode == 2)
		antiAliasingModeSelector->SetSelectedIndex(2);
	else if (currentAntiAliasingMode == 3)
		antiAliasingModeSelector->SetSelectedIndex(3);
	else if (currentAntiAliasingMode == 4)
		antiAliasingModeSelector->SetSelectedIndex(4);
}
void UOptionsMenuWidget::DetectCurrentAntiAliasingQuality() noexcept {
	int32 currentAntiAliasingQuality = gameSettings->GetAntiAliasingQuality();
	if (currentAntiAliasingQuality == 0)
		antiAliasingQualitySelector->SetSelectedIndex(0);
	else if (currentAntiAliasingQuality == 1)
		antiAliasingQualitySelector->SetSelectedIndex(1);
	else if (currentAntiAliasingQuality == 2)
		antiAliasingQualitySelector->SetSelectedIndex(2);
	else if (currentAntiAliasingQuality == 3)
		antiAliasingQualitySelector->SetSelectedIndex(3);
	else if (currentAntiAliasingQuality == 4)
		antiAliasingQualitySelector->SetSelectedIndex(4);
}
void UOptionsMenuWidget::DetectCurrentShadingQuality() noexcept {
	int32 currentShadingQuality = gameSettings->GetShadingQuality();
	if (currentShadingQuality == 0)
		shadingQualitySelector->SetSelectedIndex(0);
	else if (currentShadingQuality == 1)
		shadingQualitySelector->SetSelectedIndex(1);
	else if (currentShadingQuality == 2)
		shadingQualitySelector->SetSelectedIndex(2);
	else if (currentShadingQuality == 3)
		shadingQualitySelector->SetSelectedIndex(3);
	else if (currentShadingQuality == 4)
		shadingQualitySelector->SetSelectedIndex(4);
}
void UOptionsMenuWidget::DetectCurrentGlobalIlluminationQuality() noexcept {
	int32 currentGloblaIlluminationQuality = gameSettings->GetGlobalIlluminationQuality();
	if (currentGloblaIlluminationQuality == 0)
		globalIlluminationQualitySelector->SetSelectedIndex(0);
	else if (currentGloblaIlluminationQuality == 1)
		globalIlluminationQualitySelector->SetSelectedIndex(1);
	else if (currentGloblaIlluminationQuality == 2)
		globalIlluminationQualitySelector->SetSelectedIndex(2);
	else if (currentGloblaIlluminationQuality == 3)
		globalIlluminationQualitySelector->SetSelectedIndex(3);
	else if (currentGloblaIlluminationQuality == 4)
		globalIlluminationQualitySelector->SetSelectedIndex(4);
}
void UOptionsMenuWidget::DetectCurrentPostProcessQuality() noexcept {
	int32 currentPostProcessingQuality = gameSettings->GetPostProcessingQuality();
	if (currentPostProcessingQuality == 0)
		postProcessingQualitySelector->SetSelectedIndex(0);
	else if (currentPostProcessingQuality == 1)
		postProcessingQualitySelector->SetSelectedIndex(1);
	else if (currentPostProcessingQuality == 2)
		postProcessingQualitySelector->SetSelectedIndex(2);
	else if (currentPostProcessingQuality == 3)
		postProcessingQualitySelector->SetSelectedIndex(3);
	else if (currentPostProcessingQuality == 4)
		postProcessingQualitySelector->SetSelectedIndex(4);
}
void UOptionsMenuWidget::DetectCurrentReflectionQuality() noexcept {
	int32 currentReflectionQuality = gameSettings->GetReflectionQuality();
	if (currentReflectionQuality == 0)
		reflectionQualitySelector->SetSelectedIndex(0);
	else if (currentReflectionQuality == 1)
		reflectionQualitySelector->SetSelectedIndex(1);
	else if (currentReflectionQuality == 2)
		reflectionQualitySelector->SetSelectedIndex(2);
	else if (currentReflectionQuality == 3)
		reflectionQualitySelector->SetSelectedIndex(3);
	else if (currentReflectionQuality == 4)
		reflectionQualitySelector->SetSelectedIndex(4);
}
void UOptionsMenuWidget::DetectCurrentShadowQuality() noexcept {
	int32 currentShadowQuality = gameSettings->GetShadowQuality();
	if (currentShadowQuality == 0)
		shadowQualitySelector->SetSelectedIndex(0);
	else if (currentShadowQuality == 1)
		shadowQualitySelector->SetSelectedIndex(1);
	else if (currentShadowQuality == 2)
		shadowQualitySelector->SetSelectedIndex(2);
	else if (currentShadowQuality == 3)
		shadowQualitySelector->SetSelectedIndex(3);
	else if (currentShadowQuality == 4)
		shadowQualitySelector->SetSelectedIndex(4);
}
void UOptionsMenuWidget::DetectCurrentVisualEffectQuality() noexcept {
	int32 currentVisualEffectQuality = gameSettings->GetVisualEffectQuality();
	if (currentVisualEffectQuality == 0)
		visualEffectQualitySelector->SetSelectedIndex(0);
	else if (currentVisualEffectQuality == 1)
		visualEffectQualitySelector->SetSelectedIndex(1);
	else if (currentVisualEffectQuality == 2)
		visualEffectQualitySelector->SetSelectedIndex(2);
	else if (currentVisualEffectQuality == 3)
		visualEffectQualitySelector->SetSelectedIndex(3);
	else if (currentVisualEffectQuality == 4)
		visualEffectQualitySelector->SetSelectedIndex(4);
}
void UOptionsMenuWidget::DetectCurrentViewDistanceQuality() noexcept {
	int32 currentViewDistanceQuality = gameSettings->GetViewDistanceQuality();
	if (currentViewDistanceQuality == 0)
		viewDistanceQualitySelector->SetSelectedIndex(0);
	else if (currentViewDistanceQuality == 1)
		viewDistanceQualitySelector->SetSelectedIndex(1);
	else if (currentViewDistanceQuality == 2)
		viewDistanceQualitySelector->SetSelectedIndex(2);
	else if (currentViewDistanceQuality == 3)
		viewDistanceQualitySelector->SetSelectedIndex(3);
	else if (currentViewDistanceQuality == 4)
		viewDistanceQualitySelector->SetSelectedIndex(4);
}
void UOptionsMenuWidget::DetectCurrentFoliageQuality() noexcept {
	int32 currentFoliageQuality = gameSettings->GetFoliageQuality();
	if (currentFoliageQuality == 0)
		foliageQualitySelector->SetSelectedIndex(0);
	else if (currentFoliageQuality == 1)
		foliageQualitySelector->SetSelectedIndex(1);
	else if (currentFoliageQuality == 2)
		foliageQualitySelector->SetSelectedIndex(2);
	else if (currentFoliageQuality == 3)
		foliageQualitySelector->SetSelectedIndex(3);
	else if (currentFoliageQuality == 4)
		foliageQualitySelector->SetSelectedIndex(4);
}
void UOptionsMenuWidget::DetectCurrentAudioQuality() noexcept {
	int32 currentAudioQuality = gameSettings->GetAudioQualityLevel();
	if (currentAudioQuality == 0)
		audioQualitySelector->SetSelectedIndex(0);
	else if (currentAudioQuality == 1)
		audioQualitySelector->SetSelectedIndex(1);
	else if (currentAudioQuality == 2)
		audioQualitySelector->SetSelectedIndex(2);
	else if (currentAudioQuality == 3)
		audioQualitySelector->SetSelectedIndex(3);
	else if (currentAudioQuality == 4)
		audioQualitySelector->SetSelectedIndex(4);
}
void UOptionsMenuWidget::DetectCurrentAudioLevels() noexcept {
	masterVolumeSlider->SetValue(masterSoundMixer->SoundClassEffects[0].VolumeAdjuster);
	musicVolumeSlider->SetValue(musicSoundMixer->SoundClassEffects[0].VolumeAdjuster);
	sfxVolumeSlider->SetValue(sfxSoundMixer->SoundClassEffects[0].VolumeAdjuster);
}
void UOptionsMenuWidget::DetectCurrentKeybindings() noexcept {

	forwardKeySelector->SetSelectedKey(FInputChord(primaryPlayerControllerRef->GetForwardKey()));
	leftKeySelector->SetSelectedKey(FInputChord(primaryPlayerControllerRef->GetLeftKey()));
	backwardKeySelector->SetSelectedKey(FInputChord(primaryPlayerControllerRef->GetBackwardKey()));
	rightKeySelector->SetSelectedKey(FInputChord(primaryPlayerControllerRef->GetRightKey()));
	
	dashKeySelector->SetSelectedKey(FInputChord(primaryPlayerControllerRef->GetDashKey()));
	slideKeySelector->SetSelectedKey(FInputChord(primaryPlayerControllerRef->GetSlideKey()));
	jumpKeySelector->SetSelectedKey(FInputChord(primaryPlayerControllerRef->GetJumpKey()));
	shootKeySelector->SetSelectedKey(FInputChord(primaryPlayerControllerRef->GetShootKey()));

	weaponSwitchUpKeySelector->SetSelectedKey(FInputChord(primaryPlayerControllerRef->GetWeaponSwitchUpKey()));
	weaponSwitchDownKeySelector->SetSelectedKey(FInputChord(primaryPlayerControllerRef->GetWeaponSwitchDownKey()));
	reloadKeySelector->SetSelectedKey(FInputChord(primaryPlayerControllerRef->GetReloadKey()));
}
void UOptionsMenuWidget::DetectCurrentSensitivitySettings() noexcept {
	mouseSensitivitySlider->SetValue(primaryPlayerRef->GetPlayerMovementSystem().GetLookSensitivityModifier());
}


//Initial Setup
void UOptionsMenuWidget::SetupInitialResolutionSettings() noexcept {
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(supportedResolutions);

	for (int i = 0; i < supportedResolutions.Num(); i++) {
		FString name;
		name.AppendInt(supportedResolutions[i].X);
		name.AppendChars(" x ", 3);
		name.AppendInt(supportedResolutions[i].Y);
		resolutionOptions.Add({ name, supportedResolutions[i] });

		resolutionSelector->AddOption(name);
	}
}
void UOptionsMenuWidget::SetupInitialWindowModeSettings() noexcept {
	FString name;

	name = LexToString(EWindowMode::Type::Fullscreen);
	windowModeOptions.Add({ name, EWindowMode::Type::Fullscreen });
	windowModeSelector->AddOption(name);

	name = LexToString(EWindowMode::Type::Windowed);
	windowModeOptions.Add({ name, EWindowMode::Type::Windowed });
	windowModeSelector->AddOption(name);

	name = LexToString(EWindowMode::Type::WindowedFullscreen);
	windowModeOptions.Add({ name, EWindowMode::Type::WindowedFullscreen });
	windowModeSelector->AddOption(name);
}
void UOptionsMenuWidget::SetupInitialFramerateLimitSettings() noexcept {
	FString name;

	name = FString::FromInt(30);
	framerateLimitOptions.Add({ name, 30.0f });
	framerateLimitSelector->AddOption(name);

	name = FString::FromInt(60);
	framerateLimitOptions.Add({ name, 60.0f });
	framerateLimitSelector->AddOption(name);

	name = FString::FromInt(120);
	framerateLimitOptions.Add({ name, 120.0f });
	framerateLimitSelector->AddOption(name);

	name = FString::FromInt(144);
	framerateLimitOptions.Add({ name, 144.0f });
	framerateLimitSelector->AddOption(name);

	name = "Unlimited";
	framerateLimitOptions.Add({ name, 0.0f });
	framerateLimitSelector->AddOption(name);
}
void UOptionsMenuWidget::SetupInitialTextureQualitySettings() noexcept {
	FString name;

	name = "Low";
	textureQualityOptions.Add({ name, 0 });
	textureQualitySelector->AddOption(name);

	name = "Medium";
	textureQualityOptions.Add({ name, 1 });
	textureQualitySelector->AddOption(name);

	name = "High";
	textureQualityOptions.Add({ name, 2 });
	textureQualitySelector->AddOption(name);

	name = "Ultra";
	textureQualityOptions.Add({ name, 3 });
	textureQualitySelector->AddOption(name);

	name = "Ultra High";
	textureQualityOptions.Add({ name, 4 });
	textureQualitySelector->AddOption(name);

	//Note: This also works since the indices and the values are the same.
	//textureQualitySelector->SetSelectedIndex(gameSettings->GetTextureQuality());
}
void UOptionsMenuWidget::SetupInitialAntiAliasingModeSettings() noexcept {
	//Off - 0
	//FXAA - 1
	//TAA - 2
	//MSAA - 3
	//TSR - 4

	FString name;

	name = "Off";
	antiAliasingModeOptions.Add({ name, 0 });
	antiAliasingModeSelector->AddOption(name);

	name = "FXAA";
	antiAliasingModeOptions.Add({ name, 1 });
	antiAliasingModeSelector->AddOption(name);

	name = "TAA";
	antiAliasingModeOptions.Add({ name, 2 });
	antiAliasingModeSelector->AddOption(name);

	name = "MSAA";
	antiAliasingModeOptions.Add({ name, 3 });
	antiAliasingModeSelector->AddOption(name);

	name = "TSR";
	antiAliasingModeOptions.Add({ name, 4 });
	antiAliasingModeSelector->AddOption(name);
}
void UOptionsMenuWidget::SetupInitialAntiAliasingQualitySettings() noexcept {
	FString name;

	name = "Low";
	antiAliasingQualityOptions.Add({ name, 0 });
	antiAliasingQualitySelector->AddOption(name);

	name = "Medium";
	antiAliasingQualityOptions.Add({ name, 1 });
	antiAliasingQualitySelector->AddOption(name);

	name = "High";
	antiAliasingQualityOptions.Add({ name, 2 });
	antiAliasingQualitySelector->AddOption(name);

	name = "Ultra";
	antiAliasingQualityOptions.Add({ name, 3 });
	antiAliasingQualitySelector->AddOption(name);

	name = "Ultra High";
	antiAliasingQualityOptions.Add({ name, 4 });
	antiAliasingQualitySelector->AddOption(name);
}
void UOptionsMenuWidget::SetupInitialShadingQualitySettings() noexcept {
	FString name;

	name = "Low";
	shadingQualityOptions.Add({ name, 0 });
	shadingQualitySelector->AddOption(name);

	name = "Medium";
	shadingQualityOptions.Add({ name, 1 });
	shadingQualitySelector->AddOption(name);

	name = "High";
	shadingQualityOptions.Add({ name, 2 });
	shadingQualitySelector->AddOption(name);

	name = "Ultra";
	shadingQualityOptions.Add({ name, 3 });
	shadingQualitySelector->AddOption(name);

	name = "Ultra High";
	shadingQualityOptions.Add({ name, 4 });
	shadingQualitySelector->AddOption(name);
}
void UOptionsMenuWidget::SetupInitialGlobalIlluminationQualitySettings() noexcept {
	FString name;

	name = "Low";
	globalIlluminationQualityOptions.Add({ name, 0 });
	globalIlluminationQualitySelector->AddOption(name);

	name = "Medium";
	globalIlluminationQualityOptions.Add({ name, 1 });
	globalIlluminationQualitySelector->AddOption(name);

	name = "High";
	globalIlluminationQualityOptions.Add({ name, 2 });
	globalIlluminationQualitySelector->AddOption(name);

	name = "Ultra";
	globalIlluminationQualityOptions.Add({ name, 3 });
	globalIlluminationQualitySelector->AddOption(name);

	name = "Ultra High";
	globalIlluminationQualityOptions.Add({ name, 4 });
	globalIlluminationQualitySelector->AddOption(name);
}
void UOptionsMenuWidget::SetupInitialPostProcessingQualitySettings() noexcept {

	FString name;

	name = "Low";
	postProcessingQualityOptions.Add({ name, 0 });
	postProcessingQualitySelector->AddOption(name);

	name = "Medium";
	postProcessingQualityOptions.Add({ name, 1 });
	postProcessingQualitySelector->AddOption(name);

	name = "High";
	postProcessingQualityOptions.Add({ name, 2 });
	postProcessingQualitySelector->AddOption(name);

	name = "Ultra";
	postProcessingQualityOptions.Add({ name, 3 });
	postProcessingQualitySelector->AddOption(name);

	name = "Ultra High";
	postProcessingQualityOptions.Add({ name, 4 });
	postProcessingQualitySelector->AddOption(name);
}
void UOptionsMenuWidget::SetupInitialReflectionQualitySettings() noexcept {
	FString name;

	name = "Low";
	reflectionQualityOptions.Add({ name, 0 });
	reflectionQualitySelector->AddOption(name);

	name = "Medium";
	reflectionQualityOptions.Add({ name, 1 });
	reflectionQualitySelector->AddOption(name);

	name = "High";
	reflectionQualityOptions.Add({ name, 2 });
	reflectionQualitySelector->AddOption(name);

	name = "Ultra";
	reflectionQualityOptions.Add({ name, 3 });
	reflectionQualitySelector->AddOption(name);

	name = "Ultra High";
	reflectionQualityOptions.Add({ name, 4 });
	reflectionQualitySelector->AddOption(name);
}
void UOptionsMenuWidget::SetupInitialShadowQualitySettings() noexcept {
	FString name;

	name = "Low";
	shadowQualityOptions.Add({ name, 0 });
	shadowQualitySelector->AddOption(name);

	name = "Medium";
	shadowQualityOptions.Add({ name, 1 });
	shadowQualitySelector->AddOption(name);

	name = "High";
	shadowQualityOptions.Add({ name, 2 });
	shadowQualitySelector->AddOption(name);

	name = "Ultra";
	shadowQualityOptions.Add({ name, 3 });
	shadowQualitySelector->AddOption(name);

	name = "Ultra High";
	shadowQualityOptions.Add({ name, 4 });
	shadowQualitySelector->AddOption(name);
}
void UOptionsMenuWidget::SetupInitialVisualEffectQualitySettings() noexcept {
	FString name;

	name = "Low";
	visualEffectQualityOptions.Add({ name, 0 });
	visualEffectQualitySelector->AddOption(name);

	name = "Medium";
	visualEffectQualityOptions.Add({ name, 1 });
	visualEffectQualitySelector->AddOption(name);

	name = "High";
	visualEffectQualityOptions.Add({ name, 2 });
	visualEffectQualitySelector->AddOption(name);

	name = "Ultra";
	visualEffectQualityOptions.Add({ name, 3 });
	visualEffectQualitySelector->AddOption(name);

	name = "Ultra High";
	visualEffectQualityOptions.Add({ name, 4 });
	visualEffectQualitySelector->AddOption(name);
}
void UOptionsMenuWidget::SetupInitialViewDistanceQualitySettings() noexcept {
	FString name;

	name = "Low";
	viewDistanceQualityOptions.Add({ name, 0 });
	viewDistanceQualitySelector->AddOption(name);

	name = "Medium";
	viewDistanceQualityOptions.Add({ name, 1 });
	viewDistanceQualitySelector->AddOption(name);

	name = "High";
	viewDistanceQualityOptions.Add({ name, 2 });
	viewDistanceQualitySelector->AddOption(name);

	name = "Ultra";
	viewDistanceQualityOptions.Add({ name, 3 });
	viewDistanceQualitySelector->AddOption(name);

	name = "Ultra High";
	viewDistanceQualityOptions.Add({ name, 4 });
	viewDistanceQualitySelector->AddOption(name);
}
void UOptionsMenuWidget::SetupInitialFoliageQualitySettings() noexcept {
	FString name;

	name = "Low";
	foliageQualityOptions.Add({ name, 0 });
	foliageQualitySelector->AddOption(name);

	name = "Medium";
	foliageQualityOptions.Add({ name, 1 });
	foliageQualitySelector->AddOption(name);

	name = "High";
	foliageQualityOptions.Add({ name, 2 });
	foliageQualitySelector->AddOption(name);

	name = "Ultra";
	foliageQualityOptions.Add({ name, 3 });
	foliageQualitySelector->AddOption(name);

	name = "Ultra High";
	foliageQualityOptions.Add({ name, 4 });
	foliageQualitySelector->AddOption(name);
}
void UOptionsMenuWidget::SetupInitialAudioQualitySettings() noexcept {
	FString name;

	name = "Low";
	audioQualityOptions.Add({ name, 0 });
	audioQualitySelector->AddOption(name);

	name = "Medium";
	audioQualityOptions.Add({ name, 1 });
	audioQualitySelector->AddOption(name);

	name = "High";
	audioQualityOptions.Add({ name, 2 });
	audioQualitySelector->AddOption(name);

	name = "Ultra";
	audioQualityOptions.Add({ name, 3 });
	audioQualitySelector->AddOption(name);

	name = "Ultra High";
	audioQualityOptions.Add({ name, 4 });
	audioQualitySelector->AddOption(name);
}


//General Callbacks
void UOptionsMenuWidget::ReturnButtonClicked() {
	primaryHUDRef->SetMenuState(primaryHUDRef->GetPreviousActiveMenu()->GetMenuStateType());
}
void UOptionsMenuWidget::PopupConfirmButtonClicked() {
	gameSettings->ApplySettings(false);
	applySettingsButton->SetIsEnabled(false);
	SetConfirmationPopupState(false);
	auto type = primaryHUDRef->GetPreviousActiveMenu()->GetMenuStateType();
	primaryHUDRef->SetMenuState(type);
}
void UOptionsMenuWidget::PopupRevertButtonClicked() {
	gameSettings->ResetToCurrentSettings(); //Need a function to update all current settings upon opening the menu.
	SetConfirmationPopupState(false);
	auto type = primaryHUDRef->GetPreviousActiveMenu()->GetMenuStateType();
	primaryHUDRef->SetMenuState(type);
}
void UOptionsMenuWidget::DisplayTabButtonClicked() {
	SetActiveTab(OptionsMenuTab::DISPLAY);
}
void UOptionsMenuWidget::GraphicsTabButtonClicked() {
	SetActiveTab(OptionsMenuTab::GRAPHICS);
}
void UOptionsMenuWidget::AudioTabButtonClicked() {
	SetActiveTab(OptionsMenuTab::AUDIO);
}
void UOptionsMenuWidget::InputTabButtonClicked() {
	SetActiveTab(OptionsMenuTab::INPUT);
}


//Display Tab Callbacks
void UOptionsMenuWidget::ApplySettingsButtonClicked() {
	gameSettings->ApplySettings(false);
	applySettingsButton->SetIsEnabled(false);
}
void UOptionsMenuWidget::UpdateResolutionSelector(FString SelectedItem, ESelectInfo::Type SelectionType) {
	for (auto& option : resolutionOptions) {
		if (option.key == SelectedItem) {
			gameSettings->SetScreenResolution(option.resolution);
			applySettingsButton->SetIsEnabled(true);
			return;
		}
	}
}
void UOptionsMenuWidget::UpdateWindowModeSelector(FString SelectedItem, ESelectInfo::Type SelectionType) {
	for (auto& option : windowModeOptions) {
		if (option.key == SelectedItem) {
			gameSettings->SetFullscreenMode(option.type);
			applySettingsButton->SetIsEnabled(true);
			return;
		}
	}
}
void UOptionsMenuWidget::UpdateResolutionScaleSlider(float value) {
	FString percentage = FString::FromInt(value * 100);
	percentage.Append("%");
	resolutionScalePercentageText->SetText(FText::FromString(percentage));

	gameSettings->SetResolutionScaleNormalized(value);
	applySettingsButton->SetIsEnabled(true);
}
void UOptionsMenuWidget::UpdateDynamicResolutionCheckbox(bool bIsChecked) {
	gameSettings->SetDynamicResolutionEnabled(bIsChecked);
	applySettingsButton->SetIsEnabled(true);
}
void UOptionsMenuWidget::UpdateVSyncCheckbox(bool bIsChecked) {
	gameSettings->SetVSyncEnabled(bIsChecked);
	applySettingsButton->SetIsEnabled(true);
}
void UOptionsMenuWidget::UpdateFramerateLimitSelector(FString SelectedItem, ESelectInfo::Type SelectionType) {
	for (auto& option : framerateLimitOptions) {
		if (option.key == SelectedItem) {
			gameSettings->SetFrameRateLimit(option.value);
			applySettingsButton->SetIsEnabled(true);
			return;
		}
	}
}


//Graphics Tab Callbacks
void UOptionsMenuWidget::UpdateTextureQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType) {
	for (auto& option : textureQualityOptions) {
		if (option.key == SelectedItem) {
			gameSettings->SetTextureQuality(option.value);
			applySettingsButton->SetIsEnabled(true);
			return;
		}
	}
}
void UOptionsMenuWidget::UpdateAntiAliasingModeSelector(FString SelectedItem, ESelectInfo::Type SelectionType) {
	for (auto& option : antiAliasingModeOptions) {
		if (option.key == SelectedItem) {
			UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.AntiAliasingMethod " + FString::FromInt(option.value));
			return;
		}
	}
}
void UOptionsMenuWidget::UpdateAntiAliasingQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType) {
	for (auto& option : antiAliasingQualityOptions) {
		if (option.key == SelectedItem) {
			gameSettings->SetAntiAliasingQuality(option.value);
			applySettingsButton->SetIsEnabled(true);
			return;
		}
	}
}
void UOptionsMenuWidget::UpdateShadingQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType) {
	for (auto& option : shadingQualityOptions) {
		if (option.key == SelectedItem) {
			gameSettings->SetShadingQuality(option.value);
			applySettingsButton->SetIsEnabled(true);
			return;
		}
	}
}
void UOptionsMenuWidget::UpdateGlobalIlluminationQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType) {
	for (auto& option : globalIlluminationQualityOptions) {
		if (option.key == SelectedItem) {
			gameSettings->SetGlobalIlluminationQuality(option.value);
			applySettingsButton->SetIsEnabled(true);
			return;
		}
	}
}
void UOptionsMenuWidget::UpdatePostProcessingQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType) {
	for (auto& option : postProcessingQualityOptions) {
		if (option.key == SelectedItem) {
			gameSettings->SetPostProcessingQuality(option.value);
			applySettingsButton->SetIsEnabled(true);
			return;
		}
	}
}
void UOptionsMenuWidget::UpdateReflectionQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType) {
	for (auto& option : reflectionQualityOptions) {
		if (option.key == SelectedItem) {
			gameSettings->SetReflectionQuality(option.value);
			applySettingsButton->SetIsEnabled(true);
			return;
		}
	}
}
void UOptionsMenuWidget::UpdateShadowQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType) {
	for (auto& option : shadowQualityOptions) {
		if (option.key == SelectedItem) {
			gameSettings->SetShadowQuality(option.value);
			applySettingsButton->SetIsEnabled(true);
			return;
		}
	}
}
void UOptionsMenuWidget::UpdateVisualEffectQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType) {
	for (auto& option : visualEffectQualityOptions) {
		if (option.key == SelectedItem) {
			gameSettings->SetVisualEffectQuality(option.value);
			applySettingsButton->SetIsEnabled(true);
			return;
		}
	}
}
void UOptionsMenuWidget::UpdateViewDistanceQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType) {
	for (auto& option : viewDistanceQualityOptions) {
		if (option.key == SelectedItem) {
			gameSettings->SetViewDistanceQuality(option.value);
			applySettingsButton->SetIsEnabled(true);
			return;
		}
	}
}
void UOptionsMenuWidget::UpdateFoliageQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType) {
	for (auto& option : foliageQualityOptions) {
		if (option.key == SelectedItem) {
			gameSettings->SetFoliageQuality(option.value);
			applySettingsButton->SetIsEnabled(true);
			return;
		}
	}
}
void UOptionsMenuWidget::UpdateAudioQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType) {
	for (auto& option : audioQualityOptions) {
		if (option.key == SelectedItem) {
			gameSettings->SetAudioQualityLevel(option.value);
			applySettingsButton->SetIsEnabled(true);
			return;
		}
	}
}


//Audio Tab Callbacks
void UOptionsMenuWidget::UpdateMasterVolumeSlider(float value) {
	for (auto& mix : masterSoundMixer->SoundClassEffects)
		mix.VolumeAdjuster = value;

	GetWorld()->GetAudioDevice()->PushSoundMixModifier(masterSoundMixer);
}
void UOptionsMenuWidget::UpdateMusicVolumeSlider(float value) {
	for (auto& mix : musicSoundMixer->SoundClassEffects)
		mix.VolumeAdjuster = value;

	GetWorld()->GetAudioDevice()->PushSoundMixModifier(musicSoundMixer);
}
void UOptionsMenuWidget::UpdateSFXVolumeSlider(float value) {
	for (auto& mix : sfxSoundMixer->SoundClassEffects)
		mix.VolumeAdjuster = value;

	GetWorld()->GetAudioDevice()->PushSoundMixModifier(sfxSoundMixer);

}

//Input Tab Callbacks
void UOptionsMenuWidget::UpdateForwardKeySelector(FInputChord SelectedKey) {
	if (primaryPlayerControllerRef->GetForwardKey() == SelectedKey.Key)
		return;

	primaryPlayerControllerRef->SetForwardKey(SelectedKey.Key);
}
void UOptionsMenuWidget::UpdateBackwardKeySelector(FInputChord SelectedKey) {
	if (primaryPlayerControllerRef->GetBackwardKey() == SelectedKey.Key)
		return;

	primaryPlayerControllerRef->SetBackwardKey(SelectedKey.Key);
}
void UOptionsMenuWidget::UpdateRightKeySelector(FInputChord SelectedKey) {
	if (primaryPlayerControllerRef->GetRightKey() == SelectedKey.Key)
		return;

	primaryPlayerControllerRef->SetRightKey(SelectedKey.Key);
}
void UOptionsMenuWidget::UpdateLeftKeySelector(FInputChord SelectedKey) {
	if (primaryPlayerControllerRef->GetLeftKey() == SelectedKey.Key)
		return;

	primaryPlayerControllerRef->SetLeftKey(SelectedKey.Key);
}
void UOptionsMenuWidget::UpdateDashKeySelector(FInputChord SelectedKey) {
	if (primaryPlayerControllerRef->GetDashKey() == SelectedKey.Key)
		return;

	primaryPlayerControllerRef->SetDashKey(SelectedKey.Key);
}
void UOptionsMenuWidget::UpdateSlideKeySelector(FInputChord SelectedKey) {
	if (primaryPlayerControllerRef->GetSlideKey() == SelectedKey.Key)
		return;

	primaryPlayerControllerRef->SetSlideKey(SelectedKey.Key);
}
void UOptionsMenuWidget::UpdateJumpKeySelector(FInputChord SelectedKey) {
	if (primaryPlayerControllerRef->GetJumpKey() == SelectedKey.Key)
		return;

	primaryPlayerControllerRef->SetJumpKey(SelectedKey.Key);
}
void UOptionsMenuWidget::UpdateShootKeySelector(FInputChord SelectedKey) {
	if (primaryPlayerControllerRef->GetShootKey() == SelectedKey.Key)
		return;

	primaryPlayerControllerRef->SetShootKey(SelectedKey.Key);
}
void UOptionsMenuWidget::UpdateWeaponSwitchUpKeySelector(FInputChord SelectedKey) {
	if (primaryPlayerControllerRef->GetWeaponSwitchUpKey() == SelectedKey.Key)
		return;

	primaryPlayerControllerRef->SetWeaponSwitchUpKey(SelectedKey.Key);
}
void UOptionsMenuWidget::UpdateWeaponSwitchDownKeySelector(FInputChord SelectedKey) {
	if (primaryPlayerControllerRef->GetWeaponSwitchDownKey() == SelectedKey.Key)
		return;

	primaryPlayerControllerRef->SetWeaponSwitchDownKey(SelectedKey.Key);
}
void UOptionsMenuWidget::UpdateReloadKeySelector(FInputChord SelectedKey) {
	if (primaryPlayerControllerRef->GetReloadKey() == SelectedKey.Key)
		return;

	primaryPlayerControllerRef->SetReloadKey(SelectedKey.Key);
}
void UOptionsMenuWidget::UpdateMouseSensitivitySlider(float value) {
	primaryPlayerRef->GetPlayerMovementSystem().SetLookSensitivityModifier(value);
}