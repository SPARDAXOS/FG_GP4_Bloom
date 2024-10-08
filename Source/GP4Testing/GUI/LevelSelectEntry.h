#pragma once

#include "MenuWidgetBase.h"
#include "Blueprint/IUserObjectListEntry.h"

#include "LevelSelectEntry.generated.h"


class ULevelSelectMenuWidget;
class UImage;

class UCanvasPanel;
class UCustomButton;
struct FLevelSelectEntrySpec;


UCLASS(Abstract)
class ULevelSelectEntry : public UMenuWidgetBase, public IUserObjectListEntry {
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetSplashImage(UMaterialInterface& splash) noexcept;

public:
	void SetLevelSelectMenuReference(ULevelSelectMenuWidget& reference) noexcept { levelSelectMenuWidgetRef = &reference; }
	void SetTargetLevelSelectEntrySpec(FLevelSelectEntrySpec* spec) noexcept { targetSpec = spec; }

private:
	UFUNCTION()
	void SelectButtonClicked();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> splashImage = nullptr;

protected:
	FLevelSelectEntrySpec* targetSpec = nullptr;

private:
	ULevelSelectMenuWidget* levelSelectMenuWidgetRef = nullptr;
};