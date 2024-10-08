#pragma once

#include "MenuWidgetBase.h"
#include "LevelSelectMenuWidget.generated.h"


class UCanvasPanel;
class UCustomButton;
class UImage;
class ULevelSelectEntry;
class UTileView;



USTRUCT(BlueprintType)
struct FLevelSelectEntrySpec {

	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "EntryData")
	FName key;

	UPROPERTY(EditDefaultsOnly, Category = "EntryData")
	FName name;

	UPROPERTY(EditDefaultsOnly, Category = "EntryData")
	UMaterialInterface* splash;

	FLevelSelectEntrySpec() 
		:	key("None"), name("None"), splash(nullptr)
	{
	}
};


UCLASS(Abstract)
class ULevelSelectMenuWidget : public UMenuWidgetBase {
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

public:
	inline void SetSelectedLevelEntry(ULevelSelectEntry* entry) noexcept { selectedLevelEntry = entry; }

private:
	UFUNCTION()
	void StartButtonClicked();

	UFUNCTION()
	void ReturnButtonClicked();

private:
	void CreateLevelSelectEntries() noexcept;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> mainCanvas = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTileView> tileView = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> background = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCustomButton> startButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCustomButton> returnButton = nullptr;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ULevelSelectEntry> levelSelectEntryClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FLevelSelectEntrySpec> levelEntriesSpecs;
	
	
	
private:
	TArray<ULevelSelectEntry*> createdLevelEntries;
	ULevelSelectEntry* selectedLevelEntry = nullptr;
};