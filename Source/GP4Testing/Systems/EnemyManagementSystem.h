#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP4Testing/Systems/Entity.h"


#include "EnemyManagementSystem.generated.h"


class APrimaryGameMode;
class AWaveManager;
class AEnemyAIBase;
class AMeleeAI;
class ARangedAI;
class ATriggerVFX;


UENUM(BlueprintType)
enum class EnemyType : uint8 {
	MELEE,
	RANGED
};


UCLASS()
class AEnemyManagementSystem : public AActor, public Entity {

	GENERATED_BODY()
	
public:
	virtual void Start() override;
	virtual void Update(float deltaTime) override;

public:
	bool SpawnEnemy(EnemyType type, FVector location) noexcept;
	void DispawnAllEnemies() const noexcept;
	void DispawnMeleeEnemies() const noexcept;
	void DispawnRangedEnemies() const noexcept;

public:
	bool CreateEnemyPool(EnemyType type, uint32 count);
	void ClearPools() noexcept;

public:
	inline void SetActiveState(bool state) noexcept { active = state; }

public:
	inline void SetPrimaryGameModeReference(APrimaryGameMode& gameMode) noexcept { primaryGameModeRef = &gameMode; }
	inline void SetWaveManagerReference(AWaveManager& manager) noexcept { waveManagerRef = &manager; }

private:
	bool SpawnMeleeEnemy(const FVector&);
	bool SpawnRangedEnemy(const FVector&);
	ATriggerVFX* GetAvailableVFX() const noexcept;

private:
	bool CreateMeleeEnemiesPool(uint32 count);
	bool CreateRangedEnemiesPool(uint32 count);
	void ClearMeleeEnemiesPool() noexcept;
	void ClearRangedEnemiesPool() noexcept;
	void ValidateEnemyTypesClasses() const noexcept;
	void ValidateVFXClasses() const noexcept;

private:
	void CreateEnemySpawnPortalVFXPool();

private:
	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	TSubclassOf<ATriggerVFX> enemySpawnPortalVFXClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	int enemySpawnPortalVFXPoolSize = 20;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	float enemySpawnPortalVFXZOffset = 0.0f;

private:
	UPROPERTY(EditDefaultsOnly, Category = "EnemyTypes")
	TSubclassOf<AEnemyAIBase> meleeEnemyClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyTypes")
	TSubclassOf<AEnemyAIBase> rangedEnemyClass = nullptr;


private:
	UPROPERTY(VisibleAnywhere, Category = "Debugging")
	bool active = false;

private:
	UPROPERTY(VisibleAnywhere, Category = "Pools|VFX")
	TArray<ATriggerVFX*> enemySpawnPortalVFXPool;

	UPROPERTY(VisibleAnywhere, Category = "Pools|Enemies")
	TArray<AMeleeAI*> meleeEnemiesPool;

	UPROPERTY(VisibleAnywhere, Category = "Pools|Enemies")
	TArray<ARangedAI*> rangedEnemiesPool;

private:
	APrimaryGameMode* primaryGameModeRef = nullptr;
	AWaveManager* waveManagerRef = nullptr;
};