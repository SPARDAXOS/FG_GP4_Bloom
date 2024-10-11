#include "GP4Testing/Systems/EnemyManagementSystem.h"
#include "GP4Testing/AI/EnemyAIBase.h"
#include "GP4Testing/AI/MeleeAI.h"
#include "GP4Testing/AI/RangedAI.h"

#include "GP4Testing/Utility/Debugging.h"





void AEnemyManagementSystem::Start() {
	ValidateEnemyTypesClasses();

}
void AEnemyManagementSystem::Update(float deltaTime) {
	if (!active)
		return;


}


bool AEnemyManagementSystem::SpawnEnemy(EnemyType type, FVector3f location) noexcept {


	return true;
}
bool AEnemyManagementSystem::CreateEnemyPool(EnemyType type, uint32 count) {
	if (type == EnemyType::MELEE) {
		ClearMeleeEnemiesPool();
		CreateMeleeEnemiesPool(count);
	}
	else if (type == EnemyType::RANGED) {
		ClearRangedEnemiesPool();
		CreateRangedEnemiesPool(count);
	}
}



void AEnemyManagementSystem::ValidateEnemyTypesClasses() const noexcept {
	if (!meleeEnemyClass)
		Debugging::CustomWarning("Melee enemy class is invalid!\nEnemyManagementSystem wont be able to spawn melee enemies!");

	if (!RangedEnemyClass)
		Debugging::CustomWarning("Melee enemy class is invalid!\nEnemyManagementSystem wont be able to spawn melee enemies!");
}
bool AEnemyManagementSystem::CreateMeleeEnemiesPool(uint32 count) {
	if (!meleeEnemyClass)
		return false;

	for (int i = 0; i < count; i++) {
		AMeleeAI* newEnemy = GetWorld()->SpawnActor<AMeleeAI>(meleeEnemyClass);
		if (!newEnemy)
			continue;

		newEnemy->SetEnemyState(false);
		newEnemy->SetEnemyManagementRef(*this);
		//newEnemy->SetWaveManagerRef(waveManagerRef);
		meleeEnemiesPool.Add(newEnemy);
	}

	if (meleeEnemiesPool.Num() > 0)
		return true;
	else
		return false;
}
bool AEnemyManagementSystem::CreateRangedEnemiesPool(uint32 count) {
	if (!RangedEnemyClass)
		return false;

	for (int i = 0; i < count; i++) {
		ARangedAI* newEnemy = GetWorld()->SpawnActor<ARangedAI>(RangedEnemyClass);
		if (!newEnemy)
			continue;

		newEnemy->SetEnemyState(false);
		newEnemy->SetEnemyManagementRef(*this);
		//newEnemy->SetWaveManagerRef(waveManagerRef);
		rangedEnemiesPool.Add(newEnemy);
	}

	if (rangedEnemiesPool.Num() > 0)
		return true;
	else
		return false;
}

void AEnemyManagementSystem::ClearPools() noexcept {
	ClearMeleeEnemiesPool();
	ClearRangedEnemiesPool();
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

