#pragma once

#include "MenuWidgetBase.h"
#include "LevelSelectMenuWidget.generated.h"


class UCanvasPanel;
class UCustomButton;
class UImage;
class ULevelSelectEntry;
class ULevelSelectEntrySpec;
class UTileView;






UCLASS(Abstract)
class ULevelSelectMenuWidget : public UMenuWidgetBase {
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void SetVisibilityState(ESlateVisibility state) noexcept;

public:
	void SetSelectedLevelEntrySpec(ULevelSelectEntrySpec* entry) noexcept;

private:
	UFUNCTION()
	void StartButtonClicked();

	UFUNCTION()
	void ReturnButtonClicked();

private:
	void CreateLevelSelectEntries() noexcept;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> mainCanvas = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTileView> tileView = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> background = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCustomButton> startButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCustomButton> returnButton = nullptr;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<ULevelSelectEntrySpec*> levelSelectEntrySpecAssets;
	
	
	
protected:
	ULevelSelectEntrySpec* selectedLevelEntrySpec = nullptr;
};