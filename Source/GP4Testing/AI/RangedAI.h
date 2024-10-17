// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAIBase.h"
#include "Components/SphereComponent.h"
#include "RangedAI.generated.h"

class ARangedAIBullet;
class ATriggerVFX;

UCLASS()
class GP4TESTING_API ARangedAI : public AEnemyAIBase
{
	GENERATED_BODY()
	ARangedAI();
private:
	UPROPERTY()
	ATriggerVFX* DeathVFX;

	UFUNCTION()
	void EnemyDeath();
	
public:
	virtual void BeginPlay() override;
	virtual void Die() override;
	virtual void Attack() override;
	virtual void ResetAttack() override;
	virtual void Tick(float DeltaSeconds) override;
	bool bCanSeePlayer();

	UPROPERTY(EditDefaultsOnly)
	float VisionRange = 1000;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* DeathSound;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARangedAIBullet> Bullet;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Muzzle = nullptr;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Hitbox = nullptr;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* DeathVFXSpawnLoc = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATriggerVFX> TriggerVfx;
};
