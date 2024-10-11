

#include "GP4Testing/Systems/WaveManager.h"

#include "GP4Testing/DataAssets/WaveManagerSpec.h"
#include "GP4Testing/DataAssets/WaveSpec.h"

#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GP4Testing/AI/EnemyAIBase.h"
#include "EngineGlobals.h"
#include "AIController.h"


#include <Kismet/GameplayStatics.h>




void AWaveManager::Start() {


	bIsWaveInProgress = false;

	// why is this necessary?
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerCharacter = Cast<ACharacter>(PlayerController->GetPawn());
	}
	if (PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("PLAYER IS NOT NULL"));
		GetWorld()->GetTimerManager().SetTimer(waveDelayTimer, this, &AWaveManager::StartWave, timeBetweenWaves, false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PLAYER IS NULL"));
		return;
	}

}
void AWaveManager::Update(float deltaTime) {


	if (bIsWaveInProgress)
	{
		enemySpawnTimer -= deltaTime;

		if (enemySpawned < enemyToSpawn)
		{
			if (enemySpawnTimer <= 0.0f)
			{
				enemySpawnTimer = 5.0f;

				SpawnAI();

				enemySpawned++;
			}
		}
	}
}


bool AWaveManager::StartWave(const UWaveManagerSpec& spec) {
	//Copy it 
	activeWaveManagerSpec = &spec;

	//Copy current wave data and use it to keep track of current wave state.
	activeWaveSpecData = spec.waves[0]->data;

	return true;
}

  
void AWaveManager::StartWave()
{
	bIsWaveInProgress = true;
	enemiesAlive = 0;
	enemySpawned = 0;
	totalEnemiesKilled = 0;

	enemyToSpawn = 3 + (currentWave - 1) * SpawnAmount;
	enemySpawnTimer = 5.0f;
	
	//SpawnAIWave();
}

void AWaveManager::SpawnAIWave()
{
	if (bIsWaveInProgress)
	{
		for (int i = 0; i < enemyToSpawn; i++)
		{
			SpawnAI();
		}
	}
	
}

void AWaveManager::SpawnAI() {

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

void AWaveManager::OnAIKilled()
{
	enemiesAlive--;
	totalEnemiesKilled++;

	if (enemiesAlive <= 0 && enemySpawned >= enemyToSpawn)
	{
		bIsWaveInProgress = false;
		StartNextWave();
	}
}

void AWaveManager::StartNextWave()
{
	if (!bIsWaveInProgress)
	{
		UE_LOG(LogTemp, Warning, TEXT("ENEMIES KILLED, STARTING NEXT WAVE"));
		currentWave++;

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Current Wave: %d"), currentWave));

		GetWorld()->GetTimerManager().SetTimer(waveDelayTimer, this, &AWaveManager::StartWave, timeBetweenWaves, false);
	}
}