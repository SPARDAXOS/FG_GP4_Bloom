#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GP4Testing/DataAssets/WaveManagerSpec.h"

#include "GP4Testing/DataAssets/WaveSpecData.h"

#include "GP4Testing/Systems/Entity.h"
#include "GP4Testing/Utility/Timer.h"
#include "WaveManager.generated.h"



class APrimaryPlayerController;
class APrimaryGameMode;
class AEnemyManagementSystem;
class UWaveManagerSpec;
struct FEnemyTypeSpawnSpec;
class UWaveSpec;

enum class EnemyType : uint8;


UCLASS(Abstract)
class GP4TESTING_API AWaveManager : public AActor, public Entity {
	GENERATED_BODY()
	
public:
	virtual void Start() override;
	virtual void Update(float deltaTime) override;

public:
	bool Setup(const UWaveManagerSpec& spec);
	bool Activate() noexcept;
	void Deactivate() noexcept;
	void Restart() noexcept;

public:
	void NotifyEnemyDeath(EnemyType type);

public:
	inline void SetEnemySpawningSystemReference(AEnemyManagementSystem& system) noexcept { enemyManagementSystemRef = &system; }
	inline void SetPrimaryGameModeReference(APrimaryGameMode& gameMode) noexcept { primaryGameModeRef = &gameMode; }

	inline int GetCurrentWaveIndex() const noexcept { return currentWaveCursor; }
	inline int GetMaxWaveCount() const noexcept {
		if (!activeWaveManagerSpec)
			return 0;

		return activeWaveManagerSpec->waves.Num();
	}
	

private:
	void Clear() noexcept;
	bool StartNextWave() noexcept;
	bool SetupTimers() noexcept;
	void UpdateSpawns(EnemyType type) noexcept;
	void Completed() noexcept;

private:
	bool SpawnEnemy(const EnemyType& type, FVector location) noexcept;
	bool CreateEnemyPools();

private:
	bool IsWaveCompleted() const noexcept;
	bool GetRandomSpawnPoint(bool isOccupied, FVector& outLocation) noexcept;
	FEnemyTypeSpawnSpec* FindSpawnSpec(const EnemyType& type);
	bool ValidateAllowedEnemyTypes() const noexcept;

private:
	UPROPERTY(VisibleAnywhere, Category = "Debugging")
	bool active = false;

	UPROPERTY(VisibleAnywhere, Category = "Debugging")
	int currentWaveCursor = 0;

	UPROPERTY(VisibleAnywhere, Category = "Debugging")
	const UWaveManagerSpec* activeWaveManagerSpec = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Debugging")
	FWaveSpecData activeWaveSpecData;
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Debugging|Spawns")
	int currentTotalSpawnedEnemies = 0;

	UPROPERTY(VisibleAnywhere, Category = "Debugging|Spawns")
	int currentSpawnedMeleeEnemies = 0;

	UPROPERTY(VisibleAnywhere, Category = "Debugging|Spawns")
	int currentSpawnedSpiderEnemies = 0;

	UPROPERTY(VisibleAnywhere, Category = "Debugging|Spawns")
	int currentSpawnedRangedEnemies = 0;

public:
	UPROPERTY(EditAnywhere, Category = "Spawns")
	TArray<AActor*> spawnPoints;

	UPROPERTY(EditAnywhere, Category = "Spawns")
	int spawnPointFetchReattempts = 10;

private:
	TArray<Timer> spawnTimers;

private:
	APrimaryGameMode* primaryGameModeRef = nullptr;
	APrimaryPlayerController* primaryPlayerControllerRef = nullptr;
	AEnemyManagementSystem* enemyManagementSystemRef = nullptr;
};
