#include "GP4Testing/VFXEntities/ImpactVFX.h"
#include "NiagaraComponent.h"
#include "GP4Testing/Weapons/GunComponent.h"



AImpactVFX::AImpactVFX() {
	
	root = CreateDefaultSubobject<USceneComponent>("Root");
	VFX = CreateDefaultSubobject<UNiagaraComponent>("VFX");

	SetRootComponent(root);
	VFX->SetupAttachment(root);

	VFX->bAutoActivate = false;
}
void AImpactVFX::Activate() {
	VFX->ActivateSystem();
}
