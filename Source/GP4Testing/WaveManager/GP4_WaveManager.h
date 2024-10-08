// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GP4_WaveManager.generated.h"

UCLASS()
class GP4TESTING_API AGP4_WaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGP4_WaveManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void StartWave();

	void SpawnAIWave();


	void StartNextWave();

	void SpawnAI();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnAIKilled();

	UPROPERTY(EditAnywhere, Category = "AI SPAWNING")
	ACharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, Category = "AI SPAWNING")
	TArray<TSubclassOf<ACharacter>> AIClassToSpawn;
	
	UPROPERTY(EditAnywhere, Category = "AI SPAWNING")
	TArray<AActor*> SpawnPoints;

	UPROPERTY(EditAnywhere, Category = "AI WAVE INFO")
	int SpawnAmount;

	UPROPERTY(EditAnywhere, Category = "AI WAVE INFO")
	float timeBetweenWaves;

	UPROPERTY(VisibleAnywhere, Category = "AI WAVE INFO")
	int currentWave;
	UPROPERTY(VisibleAnywhere, Category = "AI WAVE INFO")
	int enemyToSpawn;
	UPROPERTY(VisibleAnywhere, Category = "AI WAVE INFO")
	int enemiesAlive;
	UPROPERTY(VisibleAnywhere, Category = "AI WAVE INFO")
	int totalEnemiesKilled;
	

	FTimerHandle waveDelayTimer;
};
