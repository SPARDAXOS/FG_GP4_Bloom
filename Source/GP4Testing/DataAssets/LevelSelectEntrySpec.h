#pragma once

#include "LevelSelectEntrySpec.generated.h"

class ULevelSelectMenuWidget;

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


	ULevelSelectMenuWidget* managingMenu = nullptr;
};