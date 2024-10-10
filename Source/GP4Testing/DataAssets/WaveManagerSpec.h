#pragma once

#include "WaveManagerSpec.generated.h"


class UWaveSpec;


UCLASS()
class UWaveManagerSpec : public UPrimaryDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	TArray<UWaveSpec*> waves;
};