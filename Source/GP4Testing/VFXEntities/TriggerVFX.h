#pragma once

#include "GP4Testing/Utility/Timer.h"

#include "TriggerVFX.generated.h"


class UNiagaraComponent;
class USceneComponent;

DECLARE_DELEGATE(FOnVFXFinishedSignature)

UCLASS(Abstract)
class ATriggerVFX : public AActor {
	GENERATED_BODY()

public:
	ATriggerVFX();

public:
	virtual void Tick(float deltaTime) override;

public:
	void Activate();
	void Deactivate();
	void SetupOnFinishedCallback(FOnVFXFinishedSignature callback);
	void SetupTimer(FOnVFXFinishedSignature callback, float duration);
	inline bool GetStatus() const noexcept { return status; }

private:
	UFUNCTION()
	void OnVFXFinished(UNiagaraComponent* bPSystem);
	void OnTimerFinished();

private:
	UPROPERTY(EditAnywhere)
	USceneComponent* root = nullptr;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* vfx = nullptr;

private:
	UPROPERTY(VisibleAnywhere)
	bool status = false;

private:
	Timer timer;
	FOnVFXFinishedSignature onFinishedCallback;
	FOnVFXFinishedSignature onTimerFinishedCallback;
};