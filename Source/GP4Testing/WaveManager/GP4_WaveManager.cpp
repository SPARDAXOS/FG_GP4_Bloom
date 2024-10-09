// Fill out your copyright notice in the Description page of Project Settings.


#include "GP4Testing/WaveManager/GP4_WaveManager.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GP4Testing/AI/EnemyAIBase.h"
#include "AIController.h"
#include <Kismet/GameplayStatics.h>

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
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerCharacter = Cast<ACharacter>(PlayerController->GetPawn());
	}
	if (PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("PLAYER IS NOT NULL"));
		GetWorld()->GetTimerManager().SetTimer(waveDelayTimer, this, &AGP4_WaveManager::StartWave, timeBetweenWaves, false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PLAYER IS NULL"));
		return;
	}	
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

	enemyToSpawn = 10 + (currentWave - 1) * SpawnAmount;
	SpawnAIWave();
}

void AGP4_WaveManager::SpawnAIWave()
{
	for (int i = 0; i < enemyToSpawn; i++)
	{
		SpawnAI();	
	}
}

void AGP4_WaveManager::SpawnAI()
{
	if (SpawnPoints.Num() > 0 && AIClassToSpawn.Num() > 0)
	{
		//randomize the spawn points
		int32 randIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
		AActor* spawnPoint = SpawnPoints[randIndex];

		if (!spawnPoint)
		{
			UE_LOG(LogTemp, Error, TEXT("Spawn Point is nullptr!"));
			return;
		}

		//randomize the character spawning
		int randCharacterIndex = FMath::RandRange(0, AIClassToSpawn.Num() - 1);
		TSubclassOf<ACharacter> charactersToSpawn = AIClassToSpawn[randCharacterIndex];

		FActorSpawnParameters spawnParam;
		spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		ACharacter* spawnAI = GetWorld()->SpawnActor<ACharacter>(charactersToSpawn, spawnPoint->GetActorLocation(), FRotator::ZeroRotator, spawnParam);


		if (spawnAI)
		{
			enemiesAlive++;
			UE_LOG(LogTemp, Warning, TEXT("Spawn AI: &s"), *spawnAI->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("failed to spawn AI!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("spawnpoints is 0 and AIClassToSpawn is 0"));
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

void AGP4_WaveManager::StartNextWave()
{
	UE_LOG(LogTemp, Warning, TEXT("ENEMIES KILLED, STARTING NEXT WAVE"));
	currentWave++;
	GetWorld()->GetTimerManager().SetTimer(waveDelayTimer, this, &AGP4_WaveManager::StartWave, timeBetweenWaves, false);
}