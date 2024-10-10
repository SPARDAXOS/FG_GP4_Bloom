#pragma once

#include "MenuWidgetBase.h"
#include "Blueprint/IUserObjectListEntry.h"

#include "LevelSelectEntry.generated.h"


class ULevelSelectMenuWidget;
class UImage;
class UTextBlock;

class UCanvasPanel;
class UCustomButton;
class ULevelSelectEntrySpec;


UCLASS(Abstract)
class ULevelSelectEntry : public UMenuWidgetBase, public IUserObjectListEntry {
	GENERATED_BODY()

public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool selected) override;

private:
	void SetSplashImage(UMaterialInterface& splash) noexcept;
	void SetNameText(const FName& name) noexcept;
	void SetTargetLevelSelectEntrySpec(ULevelSelectEntrySpec* spec) noexcept { targetSpec = spec; }
	void SetLevelSelectMenuReference(ULevelSelectMenuWidget& reference) noexcept { levelSelectMenuWidgetRef = &reference; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> mainCanvas = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> splashImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> nameText = nullptr;

protected:
	ULevelSelectEntrySpec* targetSpec = nullptr;

private:
	ULevelSelectMenuWidget* levelSelectMenuWidgetRef = nullptr;
};