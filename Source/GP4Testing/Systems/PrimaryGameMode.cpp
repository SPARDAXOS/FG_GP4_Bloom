

#include "GP4Testing/Systems/PrimaryGameMode.h"


//Main Systems
#include "PrimaryPlayer.h"
#include "PrimaryPlayerController.h"
#include "PrimaryHUD.h"

#include "GP4Testing/Systems/LevelManagement.h"
#include "GP4Testing/Systems/EnemyManagementSystem.h"
#include "GP4Testing/Systems/WaveManager.h"
#include "GP4Testing/DataAssets/LevelSelectEntrySpec.h"
#include "GP4Testing/DataAssets/WaveManagerSpec.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GP4Testing/Utility/Debugging.h"


APrimaryGameMode::APrimaryGameMode()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;
	audioComponent = CreateDefaultSubobject<UAudioComponent>("Audio");

}

//Init
void APrimaryGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) {
	CreateSystems();
	Super::InitGame(MapName, Options, ErrorMessage);
	BroadcastInit();
}
void APrimaryGameMode::StartPlay() {
	CacheMainSystemsReferences();
	if (!simulationMode) {
		InitMainSystems(); 	 //Requires the main systems to be cached.
		SetupDependencies();
	}
	Super::StartPlay();
	if (!simulationMode) {
		if (loadedLevelsOnGameStart.Num() == 1 && loadedLevelsOnGameStart[0] == "EmptyLevel")
			SetupPlayingState(); //sus
		else
			SetupApplicationStartState();
	}
}
void APrimaryGameMode::BeginPlay() {
	Super::BeginPlay();
	BroadcastStart();
}
void APrimaryGameMode::Tick(float deltaTime) {
	Super::Tick(deltaTime);
	UpdateGame(deltaTime);
}
void APrimaryGameMode::InitMainSystems() {
	primaryPlayerRef->Init();
	primaryHUDRef->Init();
	
}
void APrimaryGameMode::BroadcastInit() {
	//Call Init on all custom game systems
}
void APrimaryGameMode::BroadcastStart() {
	if (simulationMode)
		return;

	primaryPlayerRef->Start();
	primaryHUDRef->Start();

	waveManagerRef->Start();
	enemyManagementSystemRef->Start();
}


//Start
void APrimaryGameMode::SetupApplicationStartState() noexcept {
	if (!launchInDebugMode) {
		
		auto lambda = [this]() {
			//Main Menu Music
			PlayMainMenuMusic();
			primaryPlayerControllerRef->SetControllerInputMode(ControllerInputMode::MENU);
			primaryHUDRef->SetMenuState(MenuState::MAIN_MENU);
			currentPrimaryGameState = PrimaryGameState::MENU;
			};

		if (levelManagementRef->LoadLevel("MainMenu", lambda))
			return;

		launchInDebugMode = true;
	}

	SetupDebugModeState();
}
void APrimaryGameMode::SetupPrePlayingState() noexcept {
	//Player position, rotation adjustments
	AActor* spawnPoint = FindPlayerStart(primaryPlayerControllerRef, defaultPlayerSpawnPoint.ToString());
	if (spawnPoint) {
		primaryPlayerRef->SetActorLocation(spawnPoint->GetActorLocation(), false, nullptr, ETeleportType::ResetPhysics);
		primaryPlayerControllerRef->SetControlRotation(spawnPoint->GetActorRotation());
	}
	
	//Cutscene?



}
void APrimaryGameMode::SetupPlayingState() noexcept {
	primaryPlayerRef->SetPlayerState(true);
	primaryPlayerRef->SetPlayerHUDState(true);
	primaryPlayerRef->SetupStartingState();
	//SetupStartingState() all custom systems
	PlayGamePlayMusic();
}
void APrimaryGameMode::SetupDebugModeState() {
	SetupPrePlayingState();
	SetupPlayingState();
	primaryPlayerControllerRef->SetControllerInputMode(ControllerInputMode::GAMEPLAY);
	primaryHUDRef->ClearViewport();

	enemyManagementSystemRef->SetActiveState(true);
	if (debugModeWaveManagerSpec) {
		waveManagerRef->Setup(*debugModeWaveManagerSpec);
		waveManagerRef->Activate();
	}

	currentPrimaryGameState = PrimaryGameState::PLAYING;
	gameStarted = true;
}


//Update
void APrimaryGameMode::UpdateGame(float deltaTime) noexcept {
	if (currentPrimaryGameState == PrimaryGameState::NONE)
		return;

	UpdateStatelessSystems(deltaTime);
	switch (currentPrimaryGameState) {
	case PrimaryGameState::MENU:
		primaryHUDRef->Update(deltaTime);
		break;
	case PrimaryGameState::PAUSED:
		primaryHUDRef->Update(deltaTime);
		break;
	case PrimaryGameState::PLAYING:
		UpdatePlayingStateSystems(deltaTime);
	default:
		break;
	}
}
void APrimaryGameMode::UpdateStatelessSystems(float deltaTime) {
	//SoundSystem
}
void APrimaryGameMode::UpdatePlayingStateSystems(float deltaTime) {
	primaryPlayerRef->Update(deltaTime);
	waveManagerRef->Update(deltaTime);
	enemyManagementSystemRef->Update(deltaTime);
	//Update custom game systems
}


//Game State
bool APrimaryGameMode::StartGame(const ULevelSelectEntrySpec& spec) noexcept {
	if (gameStarted)
		return false;

	loadedLevelKey = spec.key;

	if (launchInDebugMode)
		SetupDebugModeState();
	else {
		levelManagementRef->LoadLevel(spec.key, [this, &spec]() {
			SetupPrePlayingState();
			levelManagementRef->UnloadLevel("MainMenu", [this, &spec]() {
				SetupPlayingState();
				primaryPlayerControllerRef->SetControllerInputMode(ControllerInputMode::GAMEPLAY);
				primaryHUDRef->ClearViewport(); //No transition for now

				//Activate Custom Systems
				enemyManagementSystemRef->SetActiveState(true);
				waveManagerRef->Setup(*spec.waveManagerSpec);
				waveManagerRef->Activate();
				currentPrimaryGameState = PrimaryGameState::PLAYING;
				gameStarted = true;
				});
		});
	}

	return true;
}
void APrimaryGameMode::CompleteGame(GameResults results) noexcept {
	if (results == GameResults::NONE)
		return;

	if (!gameStarted || launchInDebugMode)
		return;

	primaryPlayerControllerRef->SetControllerInputMode(ControllerInputMode::MENU);
	primaryPlayerRef->SetPlayerHUDState(false);

	currentPrimaryGameState = PrimaryGameState::MENU;
	if (results == GameResults::WIN)
		primaryHUDRef->SetMenuState(MenuState::WIN_MENU);
	else if (results == GameResults::LOSE)
		primaryHUDRef->SetMenuState(MenuState::LOSE_MENU);
}
void APrimaryGameMode::RestartGame() noexcept {
	if (!gameStarted)
		return;

	//Do i even need to restart it? maybe restart f
	waveManagerRef->Restart(); //Maybe only after an option has been selected by the menu

	SetupPrePlayingState();
	SetupPlayingState();
	primaryHUDRef->ClearViewport();
	primaryPlayerControllerRef->SetControllerInputMode(ControllerInputMode::GAMEPLAY);
	currentPrimaryGameState = PrimaryGameState::PLAYING;
}
void APrimaryGameMode::ProgressGame() noexcept {
	if (!gameStarted)
		return;

	primaryPlayerControllerRef->SetControllerInputMode(ControllerInputMode::NONE);
	primaryPlayerRef->SetPlayerState(false);
	primaryPlayerRef->SetPlayerHUDState(false);
	enemyManagementSystemRef->SetActiveState(false);
	waveManagerRef->Deactivate();

	levelManagementRef->LoadLevel("MainMenu", [this]() {
		levelManagementRef->UnloadLevel(loadedLevelKey, [this]() {
			PlayMainMenuMusic();
			primaryPlayerControllerRef->SetControllerInputMode(ControllerInputMode::MENU);
			primaryHUDRef->SetMenuState(MenuState::LEVEL_SELECT_MENU);
			currentPrimaryGameState = PrimaryGameState::MENU;
			gameStarted = false;
			});
		});
}
void APrimaryGameMode::EndGame() noexcept {
	if (!gameStarted)
		return;

	if (gamePaused)
		UnpauseGame();

	primaryPlayerControllerRef->SetControllerInputMode(ControllerInputMode::NONE);
	primaryPlayerRef->SetPlayerState(false);
	primaryPlayerRef->SetPlayerHUDState(false);

	//Disable custom systems
	enemyManagementSystemRef->SetActiveState(false);
	waveManagerRef->Deactivate();

	levelManagementRef->LoadLevel("MainMenu", [this]() {
		levelManagementRef->UnloadLevel(loadedLevelKey, [this]() {
				//Main Menu Music
				PlayMainMenuMusic();
				primaryPlayerControllerRef->SetControllerInputMode(ControllerInputMode::MENU);
				primaryHUDRef->SetMenuState(MenuState::MAIN_MENU);
				currentPrimaryGameState = PrimaryGameState::MENU;
				gameStarted = false;
			});
		});
}
void APrimaryGameMode::QuitGame() noexcept {
	UKismetSystemLibrary::QuitGame(GetWorld(), primaryPlayerControllerRef, EQuitPreference::Quit, false);
}

void APrimaryGameMode::PauseGame() noexcept {
	if (gamePaused || !gameStarted)
		return;

	gamePaused = UGameplayStatics::SetGamePaused(this, true);
	if (gamePaused) {
		audioComponent->SetPaused(true);
		primaryPlayerRef->SetPlayerHUDState(false);
		primaryHUDRef->SetMenuState(MenuState::PAUSE_MENU);
		primaryPlayerControllerRef->SetControllerInputMode(ControllerInputMode::PAUSED);
		currentPrimaryGameState = PrimaryGameState::PAUSED;
	}
}
void APrimaryGameMode::UnpauseGame() noexcept {
	if (!gamePaused || !gameStarted)
		return;

	gamePaused = UGameplayStatics::SetGamePaused(this, false);
	if (!gamePaused) {
		audioComponent->SetPaused(false);
		primaryPlayerRef->SetPlayerHUDState(true);
		primaryHUDRef->ClearViewport();
		primaryPlayerControllerRef->SetControllerInputMode(ControllerInputMode::GAMEPLAY);
		currentPrimaryGameState = PrimaryGameState::PLAYING;
	}
}

void APrimaryGameMode::PlayMainMenuMusic() {
	if (!audioComponent)
		return;

	audioComponent->SetPaused(false);
	audioComponent->Stop();
	audioComponent->SetSound(MainMenuMusic);
	audioComponent->Play();
}
void APrimaryGameMode::PlayGamePlayMusic() {
	if (!audioComponent)
		return;

	audioComponent->SetPaused(false);
	audioComponent->Stop();
	audioComponent->SetSound(GamePlayMusic);
	audioComponent->Play();
}




//Systems
void APrimaryGameMode::CreateSystems() noexcept {

	//Example
	if (levelManagementClass) {
		levelManagementRef = GetWorld()->SpawnActor<ALevelManagement>(levelManagementClass);
		if (!levelManagementRef)
			Debugging::CustomError("Failed to spawn LevelManagement actor!");
		else
			Debugging::CustomLog("LevelManagement was created successfully!");
	}
	else
		Debugging::CustomWarning("LevelManagementClass is invalid! - LevelManagement will not created!");


	if (enemyManagementSystemClass) {
		enemyManagementSystemRef = GetWorld()->SpawnActor<AEnemyManagementSystem>(enemyManagementSystemClass);
		if (!enemyManagementSystemRef)
			Debugging::CustomError("Failed to spawn EnemyManagementSystem actor!");
		else
			Debugging::CustomLog("EnemyManagementSystem was created successfully!");
	}
	else
		Debugging::CustomWarning("EnemyManagementSystemClass is invalid! - EnemyManagementSystem will not created!");


	if (waveManagerClass) {
		waveManagerRef = GetWorld()->SpawnActor<AWaveManager>(waveManagerClass);
		if (!waveManagerRef)
			Debugging::CustomError("Failed to spawn WaveManager actor!");
		else
			Debugging::CustomLog("WaveManager was created successfully!");
	}
	else
		Debugging::CustomWarning("WaveManagerClass is invalid! - WaveManager will not created!");
}
void APrimaryGameMode::SetupDependencies() noexcept {
	//Inject any dependencies required by any system.

	primaryPlayerRef->SetPrimaryGameModeReference(*this);
	primaryHUDRef->SetupDependencies(*this, *primaryPlayerRef, *primaryPlayerControllerRef);

	//CustomSystems
	enemyManagementSystemRef->SetPrimaryGameModeReference(*this);
	enemyManagementSystemRef->SetWaveManagerReference(*waveManagerRef);
	waveManagerRef->SetPrimaryGameModeReference(*this);
	waveManagerRef->SetEnemySpawningSystemReference(*enemyManagementSystemRef);
}
void APrimaryGameMode::CacheMainSystemsReferences() noexcept {
	primaryPlayerRef = Cast<APrimaryPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!primaryPlayerRef) {
		Debugging::CustomError("Failed to cache primary player ref!");
		simulationMode = true;
		return;
	}
	else
		primaryPlayerRef->SetPlayerState(false);

	primaryPlayerControllerRef = Cast<APrimaryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!primaryPlayerControllerRef) {
		Debugging::CustomError("Failed to cache primary player controller ref!");
		simulationMode = true;
		return;
	}

	if (primaryPlayerControllerRef) {
		primaryHUDRef = primaryPlayerControllerRef->GetHUD<APrimaryHUD>();
		if (!primaryHUDRef) {
			Debugging::CustomError("Failed to cache primary HUD ref!");
			simulationMode = true;
			return;
		}
	}
}