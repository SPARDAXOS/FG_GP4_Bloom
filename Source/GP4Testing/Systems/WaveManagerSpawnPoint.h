#pragma once


class UBillboardComponent;
class USceneComponent;

#include "WaveManagerSpawnPoint.generated.h"


UCLASS(Abstract)
class AWaveManagerSpawnPoint : public AActor {

	GENERATED_BODY()

public:
	AWaveManagerSpawnPoint();

protected:
	UPROPERTY(EditAnywhere)
	USceneComponent* root = nullptr;

	UPROPERTY(EditAnywhere)
	UBillboardComponent* tag = nullptr;

};