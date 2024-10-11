#pragma once


#include "TriggerVFX.generated.h"


class UNiagaraComponent;

DECLARE_DELEGATE(FOnVFXFinishedSignature)

UCLASS()
class ATriggerVFX : public AActor {
	GENERATED_BODY()

public:
	ATriggerVFX();

public:
	void Activate();
	void SetupCallback(FOnVFXFinishedSignature callback);
	inline bool GetStatus() const noexcept { return status; }

private:
	void OnVFXFinished(UNiagaraComponent* bPSystem);

private:
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* vfx = nullptr;

private:
	UPROPERTY(VisibleAnywhere)
	bool status = false;

private:
	FOnVFXFinishedSignature registeredCallback;
};