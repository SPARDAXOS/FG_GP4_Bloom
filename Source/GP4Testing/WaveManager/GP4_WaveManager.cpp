// Fill out your copyright notice in the Description page of Project Settings.


#include "GP4Testing/WaveManager/GP4_WaveManager.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "AIController.h"

// Sets default values
AGP4_WaveManager::AGP4_WaveManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGP4_WaveManager::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(waveDelayTimer, this, &AGP4_WaveManager::StartWave, timeBetweenWaves, false);

}

// Called every frame
void AGP4_WaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
  

void AGP4_WaveManager::StartWave()
{
	enemiesAlive = 0;
	totalEnemiesKilled = 0;

	//enemyToSpawn = 10 + (currentWave - 1) * 1;
	enemyToSpawn = 2;
	SpawnAIWave();
	GetWorld()->GetTimerManager().SetTimer(waveDelayTimer, this, &AGP4_WaveManager::OnAIKilled, 2, false);
}

void AGP4_WaveManager::SpawnAIWave()
{
	for (int i = 0; i < enemyToSpawn; i++)
	{
		SpawnAI();	
	}
}

void AGP4_WaveManager::StartNextWave()
{
	UE_LOG(LogTemp, Warning, TEXT("ENEMIES KILLED, STARTING NEXT WAVE"));
	currentWave++;
}

void AGP4_WaveManager::SpawnAI()
{
	if (SpawnPoints.Num() > 0 && AIClassToSpawn)
	{
		int32 randIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
		AActor* spawnPoint = SpawnPoints[randIndex];

		FActorSpawnParameters spawnParam;
		ACharacter* spawnAI = GetWorld()->SpawnActor<ACharacter>(AIClassToSpawn, spawnPoint->GetActorLocation(), FRotator::ZeroRotator, spawnParam);
		if (spawnAI)
		{
			enemiesAlive++;
		}
	}
}
void AGP4_WaveManager::OnAIKilled()
{
	enemiesAlive--;
	totalEnemiesKilled++;

	
	if (enemiesAlive <= 0)
	{
		StartNextWave();
	}
}