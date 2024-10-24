

#include "GP4Testing/Systems/WaveManager.h"
#include "GP4Testing/Systems/PrimaryGameMode.h"

#include "GP4Testing/DataAssets/WaveManagerSpec.h"
#include "GP4Testing/DataAssets/WaveSpec.h"
#include "GP4Testing/DataAssets/WaveSpecData.h"
#include "GP4Testing/Systems/EnemyManagementSystem.h"
#include "GP4Testing/Systems/WaveManagerSpawnPoint.h"


#include "Kismet/GameplayStatics.h"
#include "GP4Testing/Utility/Debugging.h"
#include "GP4Testing/AI/EnemyAIBase.h"






void AWaveManager::Start() {

}
void AWaveManager::Update(float deltaTime) {
	if (!active)
		return;





	for (auto& timer : spawnTimers)
		timer.Update(deltaTime);
}


bool AWaveManager::Setup(const UWaveManagerSpec& spec) {
	if (active) {
		Debugging::CustomWarning("Attempted to setup WaveManager while it is already active! - Setup");
		return false;
	}
	if (spec.waves.Num() == 0) {
		Debugging::CustomWarning("Spec didnt contain any wave data - Setup");
		return false;
	}

	Clear();
	activeWaveManagerSpec = &spec;
	if (!ValidateAllowedEnemyTypes())
		return false; 

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaveManagerSpawnPoint::StaticClass(), spawnPoints);
	Debugging::CustomLog(FString("WaveManager located spawn points: " + spawnPoints.Num()));
	return true;
}
bool AWaveManager::Activate() noexcept {
	if (active) {
		Debugging::CustomWarning("Attempted to activate WaveManager while it is already active! - Activate");
		return false;
	}
	if (!activeWaveManagerSpec) {
		Debugging::CustomError("Failed to activate WaveManager! - Activate");
		return false;
	}

	active = StartNextWave();
	return active;
}
void AWaveManager::Deactivate() noexcept {
	if (!active) {
		Debugging::CustomWarning("Attempted to deactivate WaveManager while it is already deactivated! - Deactivate");
		return;
	}

	Clear();
	enemyManagementSystemRef->ClearAllPools();
}
void AWaveManager::Restart() noexcept {
	if (!active)
		return;

	enemyManagementSystemRef->DespawnAllEnemies();
	enemyManagementSystemRef->DespawnAllVFX();
	currentWaveCursor = -1;
	currentSpawnedMeleeEnemies = 0;
	currentSpawnedSpiderEnemies = 0;
	currentSpawnedRangedEnemies = 0;
	currentSpawnedTyrantEnemies = 0;
	currentTotalSpawnedEnemies = 0;
	
	StartNextWave();
}


void AWaveManager::NotifyEnemyDeath(EnemyType type) {
	if (type == EnemyType::MELEE) {
		currentTotalSpawnedEnemies--;
		currentSpawnedMeleeEnemies--;
	}
	else if (type == EnemyType::SPIDER) {
		currentTotalSpawnedEnemies--;
		currentSpawnedSpiderEnemies--;
	}
	else if (type == EnemyType::RANGED) {
		currentTotalSpawnedEnemies--;
		currentSpawnedRangedEnemies--;
	}
	else if (type == EnemyType::TYRANT) {
		currentTotalSpawnedEnemies--;
		currentSpawnedTyrantEnemies--;
	}

	if (IsWaveCompleted())
		StartNextWave();
}


void AWaveManager::Clear() noexcept {
	active = false;
	activeWaveManagerSpec = nullptr;
	activeWaveSpecData = FWaveSpecData();
	currentWaveCursor = -1;
	spawnTimers.Empty();

	currentTotalSpawnedEnemies = 0;
	currentSpawnedMeleeEnemies = 0;
	currentSpawnedSpiderEnemies = 0;
	currentSpawnedRangedEnemies = 0;
	currentSpawnedTyrantEnemies = 0;

	enemyManagementSystemRef->ClearAllPools();
}
bool AWaveManager::StartNextWave() noexcept {
	if (!activeWaveManagerSpec)
		return false;

	currentWaveCursor++;
	if (currentWaveCursor >= activeWaveManagerSpec->waves.Num()) {
		Completed();
		return false;
	}

	activeWaveSpecData = activeWaveManagerSpec->waves[currentWaveCursor]->data;
	if (!SetupTimers()) {
		Debugging::CustomError("Failed to setup timers for the spawns! - Setup");
		Clear();
		return false;
	}


	if (!CreateEnemyPools()) {
		Debugging::CustomError("Failed to setup timers for the spawns! - Setup");
		Clear();
		return false;
	}

	return true;
}
void AWaveManager::UpdateSpawns(EnemyType type) noexcept {
	if (type == EnemyType::MELEE) {
		FEnemyTypeSpawnSpec* spec = FindSpawnSpec(EnemyType::MELEE);
		if (!spec) {
			Debugging::CustomError("Failed to spawn enemy with type Melee! - FindSpawnSpec() returned nullptr!");
			return;
		}

		if (spec->totalSpawns <= 0)
			return;

		if (currentSpawnedMeleeEnemies >= spec->allowedConcurentSpawns)
			return;
		
		if (currentTotalSpawnedEnemies >= activeWaveSpecData.totalAllowedConcurrentSpawns)
			return;

		FVector spawnLocation = FVector::Zero();
		if (!GetRandomSpawnPoint(false, spawnLocation)) {
			Debugging::CustomWarning("Failed to find unoccupied spawn location! - SpawnEnemy Failed!");
			return;
		}

		if (SpawnEnemy(EnemyType::MELEE, spawnLocation)) {
			currentSpawnedMeleeEnemies++;
			currentTotalSpawnedEnemies++;
		}
		else
			Debugging::CustomError("Failed to spawn enemy with type Melee! - SpawnEnemy Failed!");
	}
	else if (type == EnemyType::SPIDER) {
		FEnemyTypeSpawnSpec* spec = FindSpawnSpec(EnemyType::SPIDER);
		if (!spec) {
			Debugging::CustomError("Failed to spawn enemy with type Spider! - FindSpawnSpec() returned nullptr!");
			return;
		}

		if (spec->totalSpawns <= 0)
			return;

		if (currentSpawnedSpiderEnemies >= spec->allowedConcurentSpawns)
			return;

		if (currentTotalSpawnedEnemies >= activeWaveSpecData.totalAllowedConcurrentSpawns)
			return;

		FVector spawnLocation = FVector::Zero();
		if (!GetRandomSpawnPoint(false, spawnLocation)) {
			Debugging::CustomWarning("Failed to find unoccupied spawn location! - SpawnEnemy Failed!");
			return;
		}

		if (SpawnEnemy(EnemyType::SPIDER, spawnLocation)) {
			currentSpawnedSpiderEnemies++;
			currentTotalSpawnedEnemies++;
		}
		else
			Debugging::CustomError("Failed to spawn enemy with type Spider! - SpawnEnemy Failed!");
	}
	else if (type == EnemyType::RANGED) {
		FEnemyTypeSpawnSpec* spec = FindSpawnSpec(EnemyType::RANGED);
		if (!spec) {
			Debugging::CustomError("Failed to spawn enemy with type Ranged! - FindSpawnSpec() returned nullptr!");
			return;
		}

		if (spec->totalSpawns <= 0)
			return;

		if (currentSpawnedRangedEnemies >= spec->allowedConcurentSpawns)
			return;

		if (currentTotalSpawnedEnemies >= activeWaveSpecData.totalAllowedConcurrentSpawns)
			return;

		FVector spawnLocation = FVector::Zero();
		if (!GetRandomSpawnPoint(false, spawnLocation)) {
			Debugging::CustomWarning("Failed to find unoccupied spawn location! - SpawnEnemy Failed!");
			return;
		}

		if (SpawnEnemy(EnemyType::RANGED, spawnLocation)) {
			currentSpawnedRangedEnemies++;
			currentTotalSpawnedEnemies++;
			Debugging::CustomLog("Ranged Spawn Request Succeeded!");
		}
		else
			Debugging::CustomError("Failed to spawn enemy with type Ranged! - SpawnEnemy Failed!");
	}
	else if (type == EnemyType::TYRANT) {
		FEnemyTypeSpawnSpec* spec = FindSpawnSpec(EnemyType::TYRANT);
		if (!spec) {
			Debugging::CustomError("Failed to spawn enemy with type Tyrant! - FindSpawnSpec() returned nullptr!");
			return;
		}

		if (spec->totalSpawns <= 0)
			return;

		if (currentSpawnedTyrantEnemies >= spec->allowedConcurentSpawns)
			return;

		if (currentTotalSpawnedEnemies >= activeWaveSpecData.totalAllowedConcurrentSpawns)
			return;

		FVector spawnLocation = FVector::Zero();
		if (!GetRandomSpawnPoint(false, spawnLocation)) {
			Debugging::CustomWarning("Failed to find unoccupied spawn location! - SpawnEnemy Failed!");
			return;
		}

		if (SpawnEnemy(EnemyType::TYRANT, spawnLocation)) {
			currentSpawnedTyrantEnemies++;
			currentTotalSpawnedEnemies++;
		}
		else
			Debugging::CustomError("Failed to spawn enemy with type Tyrant! - SpawnEnemy Failed!");
	}
}
bool AWaveManager::SetupTimers() noexcept {
	if (!activeWaveManagerSpec)
		return false;
	if (activeWaveManagerSpec->waves.Num() <= 0)
		return false;
	if (activeWaveSpecData.allowedTypes.Num() <= 0) {
		Debugging::CustomError("Wave doesnt contain any allowedTypes data!\nWaveManager has been stopped!");
		return false;
	}

	spawnTimers.Empty();
	for (auto& enemyType : activeWaveSpecData.allowedTypes) {
		Timer timer;
		timer.SetOnCompletedCallback([this, &enemyType]() {UpdateSpawns(enemyType.type); });
		timer.SetLengthRef(&enemyType.spawnRate);
		spawnTimers.Emplace(timer);
	}

	return true;
}
void AWaveManager::Completed() noexcept {
	primaryGameModeRef->CompleteGame(GameResults::WIN);
}
bool AWaveManager::ValidateAllowedEnemyTypes() const noexcept {
	if (!activeWaveManagerSpec)
		return false;

	if (activeWaveManagerSpec->waves.Num() <= 0)
		return false;

	for (int i = 0; i < activeWaveManagerSpec->waves.Num(); i++) {
		UWaveSpec* currentWave = activeWaveManagerSpec->waves[i];
		TArray<EnemyType> enemyTypes;
		for (auto& typeSpawnSpec : currentWave->data.allowedTypes) {
			if (!enemyTypes.Contains(typeSpawnSpec.type))
				enemyTypes.Add(typeSpawnSpec.type);
			else{
				Debugging::CustomError("Failed to validate allowedEnemyTypes in the provided spec!\nTarget Wave: " + i);
				return false;
			}
		}
	}

	return true;
}

bool AWaveManager::IsWaveCompleted() const noexcept {
	if (!active)
		return false;

	if (currentTotalSpawnedEnemies > 0)
		return false;

	for (auto& entry : activeWaveSpecData.allowedTypes) {
		if (entry.totalSpawns > 0)
			return false;
	}

	return true;
}
FEnemyTypeSpawnSpec* AWaveManager::FindSpawnSpec(const EnemyType& type) {
	if (activeWaveSpecData.allowedTypes.Num() == 0)
		return nullptr;

	for (auto& entry : activeWaveSpecData.allowedTypes) {
		if (entry.type == type)
			return &entry;
	}

	return nullptr;
}


bool AWaveManager::SpawnEnemy(const EnemyType& type, FVector location) noexcept {
	if (!enemyManagementSystemRef)
		return false;

	bool Result = enemyManagementSystemRef->SpawnEnemy(type, location);
	if (Result) {
		FEnemyTypeSpawnSpec* spec = FindSpawnSpec(type);
		if (!spec) {
			Debugging::CustomError("Failed to update total spawns data! - Unable to find appropriate spec");
			return false;
		}

		spec->totalSpawns--;
		return true;
	}
	else
		return false;
}
bool AWaveManager::CreateEnemyPools() {
	if (!enemyManagementSystemRef)
		return false;

	enemyManagementSystemRef->ClearAllPools();
	for (auto& enemyType : activeWaveSpecData.allowedTypes) {
		if (!enemyManagementSystemRef->CreateEnemyPool(enemyType.type, enemyType.allowedConcurentSpawns))
			return false;
	}

	return true;
}
bool AWaveManager::GetRandomSpawnPoint(bool isOccupied, FVector& outLocation) noexcept {
	if (spawnPoints.Num() <= 0)
		return false;

	if (isOccupied) {
		int32 random = FMath::RandRange(0, spawnPoints.Num() - 1);
		outLocation = spawnPoints[random]->GetActorLocation();
		return true;
	}
	else {
		int spawnReattempts = 0;
		while(spawnReattempts < spawnPointFetchReattempts) {
			int32 random = FMath::RandRange(0, spawnPoints.Num() - 1);
			if (!enemyManagementSystemRef->IsSpawnPointOccupied(spawnPoints[random]->GetActorLocation())) {
				outLocation = spawnPoints[random]->GetActorLocation();
				return true;
			}

			spawnReattempts++;
		}

		return false;
	}
}

  

