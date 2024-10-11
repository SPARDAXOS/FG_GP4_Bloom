#include "EnemySpawnPortalVFX.h"
#include "NiagaraComponent.h"

AEnemySpawnPortalVFX::AEnemySpawnPortalVFX() {


	vfx = CreateDefaultSubobject<UNiagaraComponent>("VFX");
	vfx->bAutoActivate = false;
}


void AEnemySpawnPortalVFX::Activate() {
	vfx->ActivateSystem();
}