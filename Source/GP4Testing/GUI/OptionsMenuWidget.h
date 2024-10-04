#pragma once

#include "MenuWidgetBase.h"
#include "OptionsMenuWidget.generated.h"


class UCanvasPanel;
class UCustomButton;
class UImage;



UCLASS(Abstract)
class UOptionsMenuWidget : public UMenuWidgetBase {
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void ReturnButtonClicked();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> mainCanvas = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> background = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCustomButton> returnButton = nullptr;
	
};