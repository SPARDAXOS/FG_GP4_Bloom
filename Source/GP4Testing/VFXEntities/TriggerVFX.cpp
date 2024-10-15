#include "TriggerVFX.h"
#include "NiagaraComponent.h"

#include "GP4Testing/Utility/Debugging.h"

ATriggerVFX::ATriggerVFX() {

	PrimaryActorTick.bCanEverTick = true;

	vfx = CreateDefaultSubobject<UNiagaraComponent>("VFX");
	vfx->bAutoActivate = false;
	vfx->OnSystemFinished.AddDynamic(this, &ATriggerVFX::OnVFXFinished);
	timer.SetTriggerOnce(true);
	timer.SetOnCompletedCallback(std::bind(&ATriggerVFX::OnTimerFinished, this));
}



void ATriggerVFX::Tick(float deltaTime) {
	Super::Tick(deltaTime);

	if (!status)
		return;

	timer.Update(deltaTime);
}
void ATriggerVFX::SetupOnFinishedCallback(FOnVFXFinishedSignature callback) {
	if (status)
		return;

	onFinishedCallback = callback;
}
void ATriggerVFX::SetupTimer(FOnVFXFinishedSignature callback, float duration) {
	if (status)
		return;

	onTimerFinishedCallback = callback;
	timer.SetLength(duration);
}
void ATriggerVFX::Activate() {
	if (status)
		return;

	vfx->ActivateSystem();
	status = true;
}
void ATriggerVFX::OnVFXFinished(UNiagaraComponent* bPSystem) {
	onFinishedCallback.ExecuteIfBound();
	status = false;
	timer.Unlock();
}
void ATriggerVFX::OnTimerFinished() {
	onTimerFinishedCallback.ExecuteIfBound();
}