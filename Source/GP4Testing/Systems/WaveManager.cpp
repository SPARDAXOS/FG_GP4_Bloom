

#include "GP4Testing/Systems/WaveManager.h"

#include "GP4Testing/DataAssets/WaveManagerSpec.h"
#include "GP4Testing/DataAssets/WaveSpec.h"
#include "GP4Testing/DataAssets/WaveSpecData.h"
#include "GP4Testing/Systems/EnemyManagementSystem.h"



#include "GP4Testing/Utility/Debugging.h"
#include "GP4Testing/AI/EnemyAIBase.h"






void AWaveManager::Start() {


}
void AWaveManager::Update(float deltaTime) {
	if (!active)
		return;

	Debugging::CustomLog("WaveManager is updating!");
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
	if (ValidateAllowedEnemyTypes())
		return true;
	else
		return false;
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
}


void AWaveManager::NotifyEnemyDeath() {

}



void AWaveManager::Clear() noexcept {
	active = false;
	activeWaveManagerSpec = nullptr;
	activeWaveSpecData = FWaveSpecData();
	currentWaveCursor = -1;
	spawnTimers.Empty();

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

	return true;
}
void AWaveManager::UpdateSpawns(EnemyType type) noexcept {

	if (type == EnemyType::MELEE)
		Debugging::CustomLog("Spawned enemy of type 'Melee'");
	else if (type == EnemyType::RANGED)
		Debugging::CustomLog("Spawned enemy of type 'Ranged'");
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
	//Signal Gamemode that game is over and player has won!
	Clear();
}
bool AWaveManager::ValidateAllowedEnemyTypes() noexcept {
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
				Debugging::CustomWarning("Failed to validate allowedEnemyTypes in the provided spec!\nTarget Wave: " + i);
				return false;
			}
		}
	}

	return true;
}


bool AWaveManager::SpawnEnemy(const EnemyType& type, FVector& location) noexcept {

	return true;
}
FVector AWaveManager::GetRandomSpawnPoint() noexcept {

	return FVector();
}

  

