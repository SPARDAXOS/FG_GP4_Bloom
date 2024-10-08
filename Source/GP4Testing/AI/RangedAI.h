// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAIBase.h"
#include "RangedAI.generated.h"

class ARangedAIBullet;
/**
 * 
 */
UCLASS()
class GP4TESTING_API ARangedAI : public AEnemyAIBase
{
	GENERATED_BODY()
	
	virtual void Attack() override;
	virtual void ResetAttack() override;
	virtual void Tick(float DeltaSeconds) override;
	bool bCanSeePlayer();

	UPROPERTY(EditDefaultsOnly)
	float VisionRange = 1000;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARangedAIBullet> Bullet;
};
