#pragma once


#include "WaveSpecData.generated.h"



enum class EnemyType : uint8;


USTRUCT(BlueprintType)
struct FEnemyTypeSpawnSpec {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	EnemyType type;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	int totalSpawns = 60;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	int allowedConcurentSpawns = 20;
};


USTRUCT(BlueprintType)
struct FWaveSpecData {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	int totalAllowedConcurrentSpawns = 20;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	TArray<FEnemyTypeSpawnSpec> allowedTypes;
};