// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAIBase.h"
#include "MeleeAI.generated.h"

/**
 * 
 */
UCLASS()
class GP4TESTING_API AMeleeAI : public AEnemyAIBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UMaterialInterface* DissolveMat;
	
public:
	virtual void Die() override;
	virtual void SetEnemyState(bool state) override;
	virtual void Attack() override;
	virtual void ResetAttack() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	void Dissolve();
	bool bCanAttackPlayer();

	UPROPERTY(BlueprintReadOnly)
	bool bIsDead = false;

private:
	void DissolveTimer();

	UMaterialInstanceDynamic* DynMaterial;
	FTimerHandle TimerHandle;
	float DissolveValue = 0;
};
