#pragma once

#include "LevelSelectEntrySpec.generated.h"

class ULevelSelectMenuWidget;
class UWaveManagerSpec;

UCLASS()
class ULevelSelectEntrySpec : public UPrimaryDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EntryData")
	FName key = "None";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EntryData")
	FName name = "None";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EntryData")
	UMaterialInterface* splash = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EntryData")
	UWaveManagerSpec* waveManagerSpec = nullptr;


	ULevelSelectMenuWidget* managingMenu = nullptr;
};