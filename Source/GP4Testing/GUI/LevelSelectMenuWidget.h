#pragma once

#include "MenuWidgetBase.h"
#include "LevelSelectMenuWidget.generated.h"


class UCanvasPanel;
class UCustomButton;
class UImage;



UCLASS(Abstract)
class ULevelSelectMenuWidget : public UMenuWidgetBase {
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void StartButtonClicked();

	UFUNCTION()
	void ReturnButtonClicked();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> mainCanvas = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> background = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCustomButton> startButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCustomButton> returnButton = nullptr;
	
};