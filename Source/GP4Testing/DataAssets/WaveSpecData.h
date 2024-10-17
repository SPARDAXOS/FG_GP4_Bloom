#pragma once


#include "WaveSpecData.generated.h"



enum class EnemyType : uint8;


USTRUCT(BlueprintType)
struct FEnemyTypeSpawnSpec {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Data")
	EnemyType type;

	UPROPERTY(EditAnywhere, Category = "Data")
	int totalSpawns = 60;

	UPROPERTY(EditAnywhere, Category = "Data")
	int allowedConcurentSpawns = 20;

	UPROPERTY(EditAnywhere, Category = "Data")
	float spawnRate = 1.0f;
};


USTRUCT(BlueprintType)
struct FWaveSpecData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Data")
	int totalAllowedConcurrentSpawns = 20;

	UPROPERTY(EditAnywhere, Category = "Data")
	TArray<FEnemyTypeSpawnSpec> allowedTypes;
};