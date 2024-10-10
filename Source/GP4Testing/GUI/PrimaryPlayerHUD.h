#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
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
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	inline void SetPrimaryPlayerReference(APrimaryPlayer& reference) noexcept { primaryPlayerRef = &reference; }
	inline ESlateVisibility GetVisibility() const noexcept { return visible; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> mainCanvas = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> AmmoText = nullptr;

protected:
	ESlateVisibility visible;

protected:
	APrimaryPlayer* primaryPlayerRef = nullptr;
};