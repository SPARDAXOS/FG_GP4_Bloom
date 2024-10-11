#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "GP4Testing/Systems/Entity.h"

#include "PrimaryPlayerHUD.generated.h"



class APrimaryPlayer;
class APrimaryHUD;

class UCanvasPanel;


UCLASS(Abstract)
class GP4TESTING_API UPrimaryPlayerHUD : public UUserWidget, public Entity {
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

public:
	virtual void SetVisibilityState(ESlateVisibility state) noexcept;
	virtual void SetWidgetOpacity(float value) noexcept;

public:
	inline void SetPrimaryPlayerReference(APrimaryPlayer& reference) noexcept { primaryPlayerRef = &reference; }
	inline ESlateVisibility GetVisibility() const noexcept { return visible; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> mainCanvas = nullptr;

protected:
	ESlateVisibility visible;

protected:
	APrimaryPlayer* primaryPlayerRef = nullptr;
};