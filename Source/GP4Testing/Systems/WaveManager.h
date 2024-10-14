#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"

#include "GP4Testing/DataAssets/WaveSpecData.h"

#include "GP4Testing/Systems/Entity.h"
#include "GP4Testing/Utility/Timer.h"
#include "WaveManager.generated.h"



class APrimaryPlayerController;
class APrimaryGameMode;
class AEnemyManagementSystem;
class UWaveManagerSpec;
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

public:
	void NotifyEnemyDeath();

public:
	inline void SetEnemySpawningSystemReference(AEnemyManagementSystem& system) noexcept { enemyManagementSystemRef = &system; }
	inline void SetPrimaryGameModeReference(APrimaryGameMode& gameMode) noexcept { primaryGameModeRef = &gameMode; }
	inline void SetPrimaryPlayerControllerReference(APrimaryPlayerController& playerController) noexcept { primaryPlayerControllerRef = &playerController; }


private:
	void Clear() noexcept;
	bool StartNextWave() noexcept;
	bool SetupTimers() noexcept;
	void UpdateSpawns(EnemyType type) noexcept;
	void Completed() noexcept;

private:
	bool SpawnEnemy(const EnemyType& type, FVector& location) noexcept;
	FVector GetRandomSpawnPoint() noexcept;
	bool ValidateAllowedEnemyTypes() noexcept;

private:
	UPROPERTY(VisibleAnywhere, Category = "Debugging")
	bool active = false;

	UPROPERTY(VisibleAnywhere, Category = "Debugging")
	int currentWaveCursor = 0;

	UPROPERTY(VisibleAnywhere, Category = "Debugging")
	const UWaveManagerSpec* activeWaveManagerSpec = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Debugging")
	FWaveSpecData activeWaveSpecData;
	


public:
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TArray<AActor*> SpawnPoints;


private:
	TArray<Timer> spawnTimers;

private:
	APrimaryGameMode* primaryGameModeRef = nullptr;
	APrimaryPlayerController* primaryPlayerControllerRef = nullptr;
	AEnemyManagementSystem* enemyManagementSystemRef = nullptr;
};
