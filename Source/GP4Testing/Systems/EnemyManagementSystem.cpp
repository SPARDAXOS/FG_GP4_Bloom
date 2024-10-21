#include "GP4Testing/Systems/EnemyManagementSystem.h"
#include "GP4Testing/AI/EnemyAIBase.h"
#include "GP4Testing/AI/MeleeAI.h"
#include "GP4Testing/AI/RangedAI.h"
#include "GP4Testing/VFXEntities/TriggerVFX.h"
#include "Kismet/GameplayStatics.h"

#include "GP4Testing/Utility/Debugging.h"





void AEnemyManagementSystem::Start() {
	ValidateEnemyTypesClasses();
	ValidateVFXClasses();
	CreateEnemySpawnPortalVFXPool();
}
void AEnemyManagementSystem::Update(float deltaTime) {
	if (!active)
		return;


}


bool AEnemyManagementSystem::SpawnEnemy(EnemyType type, FVector location) noexcept {
	if (type == EnemyType::MELEE)
		return SpawnMeleeEnemy(location);
	else if (type == EnemyType::RANGED)
		return SpawnRangedEnemy(location);

	return false;
}
void AEnemyManagementSystem::DispawnAllEnemies() const noexcept {
	DispawnMeleeEnemies();
	DispawnRangedEnemies();
}
void AEnemyManagementSystem::DispawnMeleeEnemies() const noexcept {
	if (meleeEnemiesPool.Num() == 0)
		return;

	for (auto& enemy : meleeEnemiesPool)
		enemy->SetEnemyState(false);
}
void AEnemyManagementSystem::DispawnRangedEnemies() const noexcept {
	if (rangedEnemiesPool.Num() == 0)
		return;

	for (auto& enemy : rangedEnemiesPool)
		enemy->SetEnemyState(false);
}
bool AEnemyManagementSystem::CreateEnemyPool(EnemyType type, uint32 count) {
	if (type == EnemyType::MELEE) {
		ClearMeleeEnemiesPool();
		return CreateMeleeEnemiesPool(count);
	}
	else if (type == EnemyType::RANGED) {
		ClearRangedEnemiesPool();
		return CreateRangedEnemiesPool(count);
	}

	return false;
}
void AEnemyManagementSystem::ClearAllPools() noexcept {
	ClearMeleeEnemiesPool();
	ClearRangedEnemiesPool();
}
void AEnemyManagementSystem::DispawnAllVFX() noexcept {
	if (enemySpawnPortalVFXPool.Num() <= 0)
		return;

	for (auto& vfx : enemySpawnPortalVFXPool)
		vfx->Deactivate();

}


template<>
TArray<AEnemyAIBase*> AEnemyManagementSystem::GetAllEnemies<AEnemyAIBase>(AEnemyAIBase* self, bool excludeSelf) {
	TArray<AEnemyAIBase*> list;
	list.Reserve(meleeEnemiesPool.Num() + rangedEnemiesPool.Num());

	for (auto& entry : meleeEnemiesPool) {
		if (excludeSelf) {
			if (self == entry)
				continue;
		}

		list.Add(entry);
	}

	for (auto& entry : rangedEnemiesPool) {
		if (excludeSelf) {
			if (self == entry)
				continue;
		}

		list.Add(entry);
	}

	return list;
}
template<>
TArray<AMeleeAI*> AEnemyManagementSystem::GetAllEnemies<AMeleeAI>(AEnemyAIBase* self, bool excludeSelf) {
	TArray<AMeleeAI*> list;
	list.Reserve(meleeEnemiesPool.Num());

	for (auto& entry : meleeEnemiesPool) {
		if (excludeSelf) {
			if (self == entry)
				continue;
		}

		list.Add((AMeleeAI*)entry);
	}

	return list;
}
template<>
TArray<ARangedAI*> AEnemyManagementSystem::GetAllEnemies<ARangedAI>(AEnemyAIBase* self, bool excludeSelf) {
	TArray<ARangedAI*> list;
	list.Reserve(rangedEnemiesPool.Num());

	for (auto& entry : rangedEnemiesPool) {
		if (excludeSelf) {
			if (self == entry)
				continue;
		}

		list.Add((ARangedAI*)entry);
	}

	return list;
}


bool AEnemyManagementSystem::SpawnMeleeEnemy(FVector location) {
	if (meleeEnemiesPool.Num() == 0)
		return false;

	return SpawnEnemy_Internal(meleeEnemiesPool, location);
}
bool AEnemyManagementSystem::SpawnRangedEnemy(FVector location) {
	if (rangedEnemiesPool.Num() == 0)
		return false;

	return SpawnEnemy_Internal(rangedEnemiesPool, location);
}
bool AEnemyManagementSystem::SpawnEnemy_Internal(TArray<AEnemyAIBase*>& pool, FVector location) {
	if (pool.Num() == 0)
		return false;

	for (auto& enemy : pool) {
		if (!enemy) {
			Debugging::CustomError("Invalid enemy was found in pool!");
			continue;
		}

		if (!enemy->GetCurrentState() && !enemy->IsMarkedForSpawn()) {
			ATriggerVFX* vfx = GetAvailableVFX();
			if (vfx) {
				FOnVFXFinishedSignature callback;
				callback.BindLambda([this, &enemy, location]() {
					enemy->SetActorLocation(location, false, nullptr, ETeleportType::ResetPhysics);
					enemy->SetupStartingState();
					enemy->SetEnemyState(true);
					});
				vfx->SetupTimer(callback, enemySpawnVFXDelay);
				FVector spawnPosition = location;
				spawnPosition.Z += enemySpawnPortalVFXZOffset;
				vfx->SetActorLocation(spawnPosition);
				vfx->Activate();

				UGameplayStatics::PlaySoundAtLocation(this, PortalSound, spawnPosition);

				enemy->MarkForSpawn();
			}
			else {
				enemy->SetActorLocation(location, false, nullptr, ETeleportType::ResetPhysics);
				enemy->SetEnemyState(true);
			}
			return true;
		}
	}

	return false;
}
ATriggerVFX* AEnemyManagementSystem::GetAvailableVFX() const noexcept {
	if (enemySpawnPortalVFXPool.Num() == 0)
		return nullptr;

	for (auto& vfx : enemySpawnPortalVFXPool) {
		if (!vfx->GetStatus())
			return vfx;
	}

	return nullptr;
}
bool AEnemyManagementSystem::IsSpawnPointOccupied(FVector location) const noexcept {
	for (auto& vfx : enemySpawnPortalVFXPool) {
		if (vfx->GetActorLocation() == location && vfx->GetStatus())
			return true;
	}

	return false;
}


bool AEnemyManagementSystem::CreateMeleeEnemiesPool(uint32 count) {
	if (!meleeEnemyClass)
		return false;

	for (uint32 i = 0; i < count; i++) {
		AMeleeAI* newEnemy = GetWorld()->SpawnActor<AMeleeAI>(meleeEnemyClass);
		if (!newEnemy)
			continue;

		newEnemy->SetEnemyState(false);
		newEnemy->SetEnemyManagementRef(*this);
		newEnemy->SetWaveManagerRef(*waveManagerRef);
		newEnemy->SetEnemyType(EnemyType::MELEE);
		meleeEnemiesPool.Add(newEnemy);
	}

	if (meleeEnemiesPool.Num() > 0)
		return true;
	else
		return false;
}
bool AEnemyManagementSystem::CreateRangedEnemiesPool(uint32 count) {
	if (!rangedEnemyClass)
		return false;

	for (uint32 i = 0; i < count; i++) {
		ARangedAI* newEnemy = GetWorld()->SpawnActor<ARangedAI>(rangedEnemyClass);
		if (!newEnemy)
			continue;

		newEnemy->SetEnemyState(false);
		newEnemy->SetEnemyManagementRef(*this);
		newEnemy->SetWaveManagerRef(*waveManagerRef);
		newEnemy->SetEnemyType(EnemyType::RANGED);
		rangedEnemiesPool.Add(newEnemy);
	}

	if (rangedEnemiesPool.Num() > 0)
		return true;
	else
		return false;
}
void AEnemyManagementSystem::ClearMeleeEnemiesPool() noexcept {
	if (meleeEnemiesPool.Num() == 0)
		return;

	for (auto& entry : meleeEnemiesPool)
		entry->Destroy();

	meleeEnemiesPool.Empty();
}
void AEnemyManagementSystem::ClearRangedEnemiesPool() noexcept {
	if (rangedEnemiesPool.Num() == 0)
		return;

	for (auto& entry : rangedEnemiesPool)
		entry->Destroy();

	rangedEnemiesPool.Empty();
}
void AEnemyManagementSystem::ValidateEnemyTypesClasses() const noexcept {
	if (!meleeEnemyClass)
		Debugging::CustomWarning("Melee enemy class is invalid!\nEnemyManagementSystem wont be able to spawn melee enemies!");

	if (!rangedEnemyClass)
		Debugging::CustomWarning("Ranged enemy class is invalid!\nEnemyManagementSystem wont be able to spawn ranged enemies!");
}
void AEnemyManagementSystem::ValidateVFXClasses() const noexcept {
	if (!enemySpawnPortalVFXClass)
		Debugging::CustomWarning("enemySpawnPortalVFXClass is invalid!\nEnemyManagementSystem wont be able to enemy spawn portal VFX!");
}


void AEnemyManagementSystem::CreateEnemySpawnPortalVFXPool() {
	if (!enemySpawnPortalVFXClass)
		return;

	for (int i = 0; i < enemySpawnPortalVFXPoolSize; i++) {
		ATriggerVFX* vfx = GetWorld()->SpawnActor<ATriggerVFX>(enemySpawnPortalVFXClass);
		if (!vfx)
			continue;

		enemySpawnPortalVFXPool.Add(vfx);
	}
}



