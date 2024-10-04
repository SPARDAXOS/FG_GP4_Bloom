#pragma once

#include "MenuWidgetBase.h"
#include "MainMenuWidget.generated.h"


class UCanvasPanel;
class UCustomButton;
class UImage;



UCLASS(Abstract)
class UMainMenuWidget : public UMenuWidgetBase {
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void StartButtonClicked();

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
	TObjectPtr<UCustomButton> startButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCustomButton> optionsButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCustomButton> quitButton = nullptr;
	
};