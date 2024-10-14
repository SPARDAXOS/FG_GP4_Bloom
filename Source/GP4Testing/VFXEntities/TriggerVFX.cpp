#include "TriggerVFX.h"
#include "NiagaraComponent.h"

#include "GP4Testing/Utility/Debugging.h"

ATriggerVFX::ATriggerVFX() {


	vfx = CreateDefaultSubobject<UNiagaraComponent>("VFX");
	vfx->bAutoActivate = false;
	vfx->OnSystemFinished.AddDynamic(this, &ATriggerVFX::OnVFXFinished);
}


void ATriggerVFX::SetupCallback(FOnVFXFinishedSignature callback) {
	registeredCallback = callback;
}
void ATriggerVFX::Activate() {
	if (status)
		return;

	vfx->ActivateSystem();
	status = true;
}
void ATriggerVFX::OnVFXFinished(UNiagaraComponent* bPSystem) {
	registeredCallback.ExecuteIfBound();
	status = false;
}