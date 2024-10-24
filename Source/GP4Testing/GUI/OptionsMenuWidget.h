#pragma once

#include "MenuWidgetBase.h"
#include "OptionsMenuWidget.generated.h"


class APrimaryPlayer;
class APrimaryPlayerController;
class UGameUserSettings;
class UCanvasPanel;
class UCustomButton;
class UImage;
class USlider;
class UComboBoxString;
class UCheckBox;
class UTextBlock;
class UInputKeySelector;
class USoundSubmix;
class USoundMix;


enum class OptionsMenuBackgroundType : uint8 {
	NONE = 0,
	STATIC,
	DYNAMIC
};

enum class OptionsMenuTab : uint8 {
	NONE = 0,
	DISPLAY,
	GRAPHICS,
	AUDIO,
	INPUT
};


struct ResolutionOption {
	FString key;
	FIntPoint resolution;
};
struct WindowModeOption {
	FString key;
	EWindowMode::Type type;
};
struct FramerateLimitOption {
	FString key;
	float value;
};
struct TextureQualityOption {
	FString key;
	int value;
};
struct AntiAliasingModeOption {
	FString key;
	int value;
};
struct AntiAliasingQualityOption {
	FString key;
	int value;
};
struct ShadingQualityOption {
	FString key;
	int value;
};
struct GlobalIlluminationQualityOption {
	FString key;
	int value;
};
struct PostProcessingQualityOption {
	FString key;
	int value;
};
struct ReflectionQualityOption {
	FString key;
	int value;
};
struct ShadowQualityOption {
	FString key;
	int value;
};
struct VisualEffectQualityOption {
	FString key;
	int value;
};
struct ViewDistanceQualityOption {
	FString key;
	int value;
};
struct FoliageQualityOption {
	FString key;
	int value;
};
struct AudioQualityOption {
	FString key;
	int value;
};


UCLASS(Abstract)
class UOptionsMenuWidget : public UMenuWidgetBase {
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void SetVisibilityState(ESlateVisibility state) noexcept override;
	virtual void SetWidgetOpacity(float opacity) noexcept override;

public:
	inline void SetPrimaryPlayerReference(APrimaryPlayer& player) noexcept { primaryPlayerRef = &player; }
	inline void SetPrimaryPlayerControllerReference(APrimaryPlayerController& controller) noexcept { primaryPlayerControllerRef = &controller; }

public:
	void SetBackgroundType(OptionsMenuBackgroundType type) noexcept;
	void SetActiveTab(OptionsMenuTab tab) noexcept;
	void SetConfirmationPopupState(bool state) noexcept;

private:
	void DetectCurrentSettings() noexcept;
	void DetectCurrentResolution() noexcept;
	void DetectCurrentWindowMode() noexcept;
	void DetectCurrentResolutionScale() noexcept;
	void DetectCurrentDynamicResolution() noexcept;
	void DetectCurrentVSync() noexcept;
	void DetectCurrentFramerateLimit() noexcept;
	void DetectCurrentTextureQuality() noexcept;
	void DetectCurrentAntiAliasingMode() noexcept;
	void DetectCurrentAntiAliasingQuality() noexcept;
	void DetectCurrentShadingQuality() noexcept;
	void DetectCurrentGlobalIlluminationQuality() noexcept;
	void DetectCurrentPostProcessQuality() noexcept;
	void DetectCurrentReflectionQuality() noexcept;
	void DetectCurrentShadowQuality() noexcept;
	void DetectCurrentVisualEffectQuality() noexcept;
	void DetectCurrentViewDistanceQuality() noexcept;
	void DetectCurrentFoliageQuality() noexcept;
	void DetectCurrentAudioQuality() noexcept;
	void DetectCurrentAudioLevels() noexcept;
	void DetectCurrentKeybindings() noexcept;
	void DetectCurrentSensitivitySettings() noexcept;

private:
	void SetupInitialResolutionSettings() noexcept;
	void SetupInitialWindowModeSettings() noexcept;
	void SetupInitialFramerateLimitSettings() noexcept;
	void SetupInitialTextureQualitySettings() noexcept;
	void SetupInitialAntiAliasingModeSettings() noexcept;
	void SetupInitialAntiAliasingQualitySettings() noexcept;
	void SetupInitialShadingQualitySettings() noexcept;
	void SetupInitialGlobalIlluminationQualitySettings() noexcept;
	void SetupInitialPostProcessingQualitySettings() noexcept;
	void SetupInitialReflectionQualitySettings() noexcept;
	void SetupInitialShadowQualitySettings() noexcept;
	void SetupInitialVisualEffectQualitySettings() noexcept;
	void SetupInitialViewDistanceQualitySettings() noexcept;
	void SetupInitialFoliageQualitySettings() noexcept;
	void SetupInitialAudioQualitySettings() noexcept;

private: //General Callbacks
	UFUNCTION()
	void ReturnButtonClicked();

	UFUNCTION()
	void PopupConfirmButtonClicked();

	UFUNCTION()
	void PopupRevertButtonClicked();

	UFUNCTION()
	void DisplayTabButtonClicked();

	UFUNCTION()
	void GraphicsTabButtonClicked();

	UFUNCTION()
	void AudioTabButtonClicked();

	UFUNCTION()
	void InputTabButtonClicked();

private: //Display Tab Callbacks
	UFUNCTION()
	void ApplySettingsButtonClicked();

	UFUNCTION()
	void UpdateResolutionSelector(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void UpdateWindowModeSelector(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void UpdateResolutionScaleSlider(float value);

	UFUNCTION()
	void UpdateDynamicResolutionCheckbox(bool bIsChecked);

	UFUNCTION()
	void UpdateVSyncCheckbox(bool bIsChecked);

	UFUNCTION()
	void UpdateFramerateLimitSelector(FString SelectedItem, ESelectInfo::Type SelectionType);

private: //Graphics Tab Callbacks
	UFUNCTION()
	void UpdateTextureQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void UpdateAntiAliasingModeSelector(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void UpdateAntiAliasingQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void UpdateShadingQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void UpdateGlobalIlluminationQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void UpdatePostProcessingQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void UpdateReflectionQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void UpdateShadowQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void UpdateVisualEffectQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void UpdateViewDistanceQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void UpdateFoliageQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void UpdateAudioQualitySelector(FString SelectedItem, ESelectInfo::Type SelectionType);

private: //Audio Tab Callbacks
	UFUNCTION()
	void UpdateMasterVolumeSlider(float value);

	UFUNCTION()
	void UpdateMusicVolumeSlider(float value);

	UFUNCTION()
	void UpdateSFXVolumeSlider(float value);

private: //Input Tab Callbacks
	UFUNCTION()
	void UpdateForwardKeySelector(FInputChord SelectedKey);

	UFUNCTION()
	void UpdateBackwardKeySelector(FInputChord SelectedKey);

	UFUNCTION()
	void UpdateRightKeySelector(FInputChord SelectedKey);

	UFUNCTION()
	void UpdateLeftKeySelector(FInputChord SelectedKey);

	UFUNCTION()
	void UpdateDashKeySelector(FInputChord SelectedKey);

	UFUNCTION()
	void UpdateSlideKeySelector(FInputChord SelectedKey);

	UFUNCTION()
	void UpdateJumpKeySelector(FInputChord SelectedKey);

	UFUNCTION()
	void UpdateShootKeySelector(FInputChord SelectedKey);

	UFUNCTION()
	void UpdateWeaponSwitchUpKeySelector(FInputChord SelectedKey);

	UFUNCTION()
	void UpdateWeaponSwitchDownKeySelector(FInputChord SelectedKey);

	UFUNCTION()
	void UpdateReloadKeySelector(FInputChord SelectedKey);

	UFUNCTION()
	void UpdateMouseSensitivitySlider(float value);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> mainCanvas = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> dynamicBackground = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> staticBackground = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCustomButton> returnButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> confirmationPopupCanvas = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> displayTabCanvas = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> graphicsTabCanvas = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> audioTabCanvas = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> inputTabCanvas = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCustomButton> applySettingsButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCustomButton> popupConfirmButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCustomButton> popupRevertButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCustomButton> displayTabButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCustomButton> graphicsTabButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCustomButton> audioTabButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCustomButton> inputTabButton = nullptr;

protected: //Display Tab
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UComboBoxString> resolutionSelector = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UComboBoxString> windowModeSelector = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USlider> resolutionScaleSlider = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> resolutionScalePercentageText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCheckBox> dynamicResolutionCheckbox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCheckBox> vsyncCheckbox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UComboBoxString> framerateLimitSelector = nullptr;

protected: //Graphics Tab
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UComboBoxString> textureQualitySelector = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UComboBoxString> antiAliasingModeSelector = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UComboBoxString> antiAliasingQualitySelector = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UComboBoxString> shadingQualitySelector = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UComboBoxString> globalIlluminationQualitySelector = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UComboBoxString> postProcessingQualitySelector = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UComboBoxString> reflectionQualitySelector = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UComboBoxString> shadowQualitySelector = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UComboBoxString> visualEffectQualitySelector = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UComboBoxString> viewDistanceQualitySelector = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UComboBoxString> foliageQualitySelector = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UComboBoxString> audioQualitySelector = nullptr;

protected: //Audio Tab
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USlider> masterVolumeSlider = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USlider> musicVolumeSlider = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USlider> sfxVolumeSlider = nullptr;

protected: //Input Tab
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInputKeySelector> forwardKeySelector = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInputKeySelector> backwardKeySelector = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInputKeySelector> rightKeySelector = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInputKeySelector> leftKeySelector = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInputKeySelector> dashKeySelector = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInputKeySelector> slideKeySelector = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInputKeySelector> jumpKeySelector = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInputKeySelector> shootKeySelector = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInputKeySelector> weaponSwitchUpKeySelector = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInputKeySelector> weaponSwitchDownKeySelector = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInputKeySelector> reloadKeySelector = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USlider> mouseSensitivitySlider = nullptr;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundMix> masterSoundMixer = nullptr;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundMix> musicSoundMixer = nullptr;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundMix> sfxSoundMixer = nullptr;

private:
	UGameUserSettings* gameSettings = nullptr;
	TArray<FIntPoint> supportedResolutions;

private:
	TArray<ResolutionOption> resolutionOptions;
	TArray<WindowModeOption> windowModeOptions;
	TArray<FramerateLimitOption> framerateLimitOptions;

private:
	TArray<TextureQualityOption> textureQualityOptions;
	TArray<AntiAliasingModeOption> antiAliasingModeOptions;
	TArray<AntiAliasingQualityOption> antiAliasingQualityOptions;
	TArray<ShadingQualityOption> shadingQualityOptions;
	TArray<GlobalIlluminationQualityOption> globalIlluminationQualityOptions;
	TArray<PostProcessingQualityOption> postProcessingQualityOptions;
	TArray<ReflectionQualityOption> reflectionQualityOptions;
	TArray<ShadowQualityOption> shadowQualityOptions;
	TArray<VisualEffectQualityOption> visualEffectQualityOptions;
	TArray<ViewDistanceQualityOption> viewDistanceQualityOptions;
	TArray<FoliageQualityOption> foliageQualityOptions;
	TArray<AudioQualityOption> audioQualityOptions;

private:
	APrimaryPlayer* primaryPlayerRef = nullptr;
	APrimaryPlayerController* primaryPlayerControllerRef = nullptr;
	UMaterialInstanceDynamic* backgroundDynamicMaterial = nullptr;
	OptionsMenuBackgroundType currentBackgroundType = OptionsMenuBackgroundType::DYNAMIC;
	OptionsMenuTab currentOpenTab = OptionsMenuTab::NONE;
};