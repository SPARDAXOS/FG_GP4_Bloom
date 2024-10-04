#pragma once

#include "MenuWidgetBase.h"
#include "PauseMenuWidget.generated.h"


class UCanvasPanel;
class UCustomButton;
class UImage;



UCLASS(Abstract)
class UPauseMenuWidget : public UMenuWidgetBase {
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void ContinueButtonClicked();

	UFUNCTION()
	void OptionsButtonClicked();

	UFUNCTION()
	void QuitButtonClicked();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> mainCanvas = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> background = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCustomButton> continueButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCustomButton> optionsButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCustomButton> quitButton = nullptr;
	
};