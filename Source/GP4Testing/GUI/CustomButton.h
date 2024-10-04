#pragma once

#include "Blueprint/UserWidget.h"

#include "CustomButton.generated.h"


class UButton;
class UTextBlock;


UCLASS(Abstract)
class UCustomButton : public UUserWidget {
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void ButtonClicked();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> button = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> text = nullptr;
};
