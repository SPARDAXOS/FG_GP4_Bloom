#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP4Testing/Systems/Entity.h"


#include "EnemyManagementSystem.generated.h"


class APrimaryGameMode;


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
	//bool SpawnEnemy(EnemyType, Location);


public:
	inline void SetPrimaryGameModeReference(APrimaryGameMode& gameMode) noexcept { primaryGameModeRef = &gameMode; }

	inline void SetActiveState(bool state) noexcept { active = state; }

private:
	UPROPERTY(VisibleAnywhere, Category = "Debugging")
	bool active = false;

private:
	APrimaryGameMode* primaryGameModeRef = nullptr;

};