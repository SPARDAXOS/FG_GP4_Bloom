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
	bool SpawnEnemy(EnemyType type, FVector3f location) noexcept;
	bool CreateEnemyPool(EnemyType type, uint32 count);
	void ClearPools() noexcept;
	inline void SetActiveState(bool state) noexcept { active = state; }

public:
	inline void SetPrimaryGameModeReference(APrimaryGameMode& gameMode) noexcept { primaryGameModeRef = &gameMode; }
	inline void SetWaveManagerReference(AWaveManager& manager) noexcept { waveManagerRef = &manager; }

private:
	bool SpawnMeleeEnemy(const FVector3f&);
	bool SpawnRangedEnemy(const FVector3f&);
	bool CreateMeleeEnemiesPool(uint32 count);
	bool CreateRangedEnemiesPool(uint32 count);
	void ClearMeleeEnemiesPool() noexcept;
	void ClearRangedEnemiesPool() noexcept;
	void ValidateEnemyTypesClasses() const noexcept;

private:
	UPROPERTY(EditDefaultsOnly, Category = "EnemyTypes")
	TSubclassOf<AEnemyAIBase> meleeEnemyClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyTypes")
	TSubclassOf<AEnemyAIBase> RangedEnemyClass = nullptr;


private:
	UPROPERTY(VisibleAnywhere, Category = "Debugging")
	bool active = false;

private:
	UPROPERTY(VisibleAnywhere, Category = "Pools")
	TArray<AMeleeAI*> meleeEnemiesPool;

	UPROPERTY(VisibleAnywhere, Category = "Pools")
	TArray<ARangedAI*> rangedEnemiesPool;

private:
	APrimaryGameMode* primaryGameModeRef = nullptr;
	AWaveManager* waveManagerRef = nullptr;
};