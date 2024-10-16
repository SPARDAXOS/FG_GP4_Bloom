#pragma once

#include "MenuWidgetBase.h"
#include "WinMenuWidget.generated.h"


class UCanvasPanel;
class UCustomButton;
class UImage;



UCLASS(Abstract)
class UWinMenuWidget : public UMenuWidgetBase {
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void NextButtonClicked();

	UFUNCTION()
	void QuitButtonClicked();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> mainCanvas = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> background = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCustomButton> nextButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCustomButton> quitButton = nullptr;
	
};