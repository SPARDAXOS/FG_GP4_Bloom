#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"

#include "GP4Testing/DataAssets/WaveSpecData.h"

#include "GP4Testing/Systems/Entity.h"
#include "WaveManager.generated.h"



class APrimaryPlayerController;
class APrimaryGameMode;
class AEnemyManagementSystem;
class UWaveManagerSpec;
class UWaveSpec;


UCLASS(Abstract)
class GP4TESTING_API AWaveManager : public AActor, public Entity {
	GENERATED_BODY()
	
public:
	virtual void Start() override;
	virtual void Update(float deltaTime) override;

public:
	bool StartWave(const UWaveManagerSpec& spec);


public:
	inline void SetEnemySpawningSystemReference(AEnemyManagementSystem& system) noexcept { enemyManagementSystemRef = &system; }
	inline void SetPrimaryGameModeReference(APrimaryGameMode& gameMode) noexcept { primaryGameModeRef = &gameMode; }
	inline void SetPrimaryPlayerControllerReference(APrimaryPlayerController& playerController) noexcept { primaryPlayerControllerRef = &playerController; }

	inline void SetActiveState(bool state) noexcept { active = state; }

private:
	void StartWave();
	void SpawnAIWave();
	void StartNextWave();
	void SpawnAI();
	void SpawnTimer(float deltatime);

	FVector3f GetRandomSpawnPoint();

public:	
	void OnAIKilled();
	

private:
	UPROPERTY(VisibleAnywhere, Category = "Debugging")
	bool active = false;

	UPROPERTY(VisibleAnywhere, Category = "Debugging")
	const UWaveManagerSpec* activeWaveManagerSpec = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Debugging")
	FWaveSpecData activeWaveSpecData;
	


public:
	UPROPERTY(EditAnywhere, Category = "AI SPAWNING")
	ACharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, Category = "AI SPAWNING")
	TArray<TSubclassOf<ACharacter>> AIClassToSpawn;
	
	UPROPERTY(EditAnywhere, Category = "AI SPAWNING")
	TArray<AActor*> SpawnPoints;

	UPROPERTY(EditAnywhere, Category = "AI WAVE INFO")
	int SpawnAmount;

	UPROPERTY(EditAnywhere, Category = "AI WAVE INFO")
	float timeBetweenWaves;

	UPROPERTY(VisibleAnywhere, Category = "AI WAVE INFO")
	int currentWave = 1;
	UPROPERTY(VisibleAnywhere, Category = "AI WAVE INFO")
	int enemyToSpawn;
	UPROPERTY(VisibleAnywhere, Category = "AI WAVE INFO")
	int enemySpawned;
	UPROPERTY(VisibleAnywhere, Category = "AI WAVE INFO")
	int enemiesAlive;
	UPROPERTY(VisibleAnywhere, Category = "AI WAVE INFO")
	int totalEnemiesKilled;
	UPROPERTY(VisibleAnywhere, Category = "AI WAVE INFO")
	float enemySpawnTimer;
	UPROPERTY(VisibleAnywhere, Category = "AI WAVE INFO")
	bool bIsWaveInProgress;
	

	FTimerHandle waveDelayTimer;

private:
	APrimaryGameMode* primaryGameModeRef = nullptr;
	APrimaryPlayerController* primaryPlayerControllerRef = nullptr;
	AEnemyManagementSystem* enemyManagementSystemRef = nullptr;
};
