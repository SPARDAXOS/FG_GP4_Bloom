#pragma once


#include "WaveSpecData.h"
#include "WaveSpec.generated.h"






UCLASS()
class UWaveSpec : public UPrimaryDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FWaveSpecData data;
};