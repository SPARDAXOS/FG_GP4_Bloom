#pragma once


#include "EnemySpawnPortalVFX.generated.h"


class UNiagaraComponent;

UCLASS()
class AEnemySpawnPortalVFX : public AActor {
	GENERATED_BODY()

public:
	AEnemySpawnPortalVFX();

public:
	void Activate();

private:
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* vfx = nullptr;

};