#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP4Testing/Systems/Entity.h"


#include "EnemyManagementSystem.generated.h"


class APrimaryGameMode;
class AWaveManager;
class AEnemyAIBase;
class ATriggerVFX;
class AMeleeAI;
class ARangedAI;
class ASpiderAI;
class ATyrantAI;

UENUM(BlueprintType)
enum class EnemyType : uint8 {
	MELEE,
	SPIDER,
	RANGED,
	TYRANT
};


UCLASS()
class AEnemyManagementSystem : public AActor, public Entity {

	GENERATED_BODY()
	
public:
	virtual void Start() override;
	virtual void Update(float deltaTime) override;

public:
	bool SpawnEnemy(EnemyType type, FVector location) noexcept;
	void DespawnAllEnemies() const noexcept;
	void DespawnMeleeEnemies() const noexcept;
	void DespawnSpiderEnemies() const noexcept;
	void DespawnRangedEnemies() const noexcept;
	void DespawnTyrantEnemies() const noexcept;

public:
	bool CreateEnemyPool(EnemyType type, uint32 count);
	void ClearAllPools() noexcept;
	void DespawnAllVFX() noexcept;
	bool IsSpawnPointOccupied(FVector location) const noexcept;

public:
	template<typename T>
	TArray<T*> GetAllEnemies(AEnemyAIBase* self, bool excludeSelf);

	template<>
	TArray<AEnemyAIBase*> GetAllEnemies<AEnemyAIBase>(AEnemyAIBase* self, bool excludeSelf);

	template<>
	TArray<AMeleeAI*> GetAllEnemies<AMeleeAI>(AEnemyAIBase* self, bool excludeSelf);

	template<>
	TArray<ARangedAI*> GetAllEnemies<ARangedAI>(AEnemyAIBase* self, bool excludeSelf);

	template<>
	TArray<ASpiderAI*> GetAllEnemies<ASpiderAI>(AEnemyAIBase* self, bool excludeSelf);

	template<>
	TArray<ATyrantAI*> GetAllEnemies<ATyrantAI>(AEnemyAIBase* self, bool excludeSelf);
	


public:
	inline void SetActiveState(bool state) noexcept { active = state; }

public:
	inline void SetPrimaryGameModeReference(APrimaryGameMode& gameMode) noexcept { primaryGameModeRef = &gameMode; }
	inline void SetWaveManagerReference(AWaveManager& manager) noexcept { waveManagerRef = &manager; }

private:
	bool SpawnMeleeEnemy(FVector location);
	bool SpawnSpiderEnemy(FVector location);
	bool SpawnRangedEnemy(FVector location);
	bool SpawnTyrantEnemy(FVector location);
	bool SpawnEnemy_Internal(TArray<AEnemyAIBase*>& pool, FVector location);
	ATriggerVFX* GetAvailableVFX() const noexcept;

private:
	bool CreateMeleeEnemiesPool(uint32 count);
	bool CreateSpiderEnemiesPool(uint32 count);
	bool CreateRangedEnemiesPool(uint32 count);
	bool CreateTyrantEnemiesPool(uint32 count);
	void ClearMeleeEnemiesPool() noexcept;
	void ClearSpiderEnemiesPool() noexcept;
	void ClearRangedEnemiesPool() noexcept;
	void ClearTyrantEnemiesPool() noexcept;
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

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	float enemySpawnVFXDelay = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase* PortalSound;

private:
	UPROPERTY(EditDefaultsOnly, Category = "EnemyTypes")
	TSubclassOf<AEnemyAIBase> meleeEnemyClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyTypes")
	TSubclassOf<AEnemyAIBase> spiderEnemyClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyTypes")
	TSubclassOf<AEnemyAIBase> rangedEnemyClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyTypes")
	TSubclassOf<AEnemyAIBase> tyrantEnemyClass = nullptr;

private:
	UPROPERTY(VisibleAnywhere, Category = "Debugging")
	bool active = false;

private:
	UPROPERTY(VisibleAnywhere, Category = "Pools|VFX")
	TArray<ATriggerVFX*> enemySpawnPortalVFXPool;

	UPROPERTY(VisibleAnywhere, Category = "Pools|Enemies")
	TArray<AEnemyAIBase*> meleeEnemiesPool;

	UPROPERTY(VisibleAnywhere, Category = "Pools|Enemies")
	TArray<AEnemyAIBase*> spiderEnemiesPool;

	UPROPERTY(VisibleAnywhere, Category = "Pools|Enemies")
	TArray<AEnemyAIBase*> rangedEnemiesPool;

	UPROPERTY(VisibleAnywhere, Category = "Pools|Enemies")
	TArray<AEnemyAIBase*> tyrantEnemiesPool;

private:
	APrimaryGameMode* primaryGameModeRef = nullptr;
	AWaveManager* waveManagerRef = nullptr;
};