

#include "GP4Testing/Systems/PrimaryGameMode.h"


//Main Systems
#include "PrimaryPlayer.h"
#include "PrimaryPlayerController.h"
#include "PrimaryHUD.h"

#include "LevelManagement.h"
#include "GP4TestinG/DataAssets/LevelSelectEntrySpec.h"


#include "Kismet/GameplayStatics.h"
#include "GP4Testing/Utility/Debugging.h"


APrimaryGameMode::APrimaryGameMode()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;


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
}


//Start
void APrimaryGameMode::SetupApplicationStartState() noexcept {

	if (launchInDebugMode) {
		SetupPrePlayingState();
		SetupPlayingState();
		primaryPlayerControllerRef->SetControllerInputMode(ControllerInputMode::GAMEPLAY);

	}
	else {
		//Main Menu Music
		primaryPlayerControllerRef->SetControllerInputMode(ControllerInputMode::MENU);
		primaryHUDRef->SetMenuState(MenuState::MAIN_MENU);
	}
}
void APrimaryGameMode::SetupPrePlayingState() noexcept {
	primaryPlayerRef->SetupStartingState();
	primaryPlayerRef->SetPlayerState(true);
	primaryPlayerRef->SetPlayerHUDState(true);

	//SetupStartingState() all custom systems

}
void APrimaryGameMode::SetupPlayingState() noexcept {
	//Player position, rotation adjustments
	//Cutscene?
}


//Update
void APrimaryGameMode::UpdateGame(float deltaTime) noexcept {
	if (currentPrimaryGameState == PrimaryGameState::NONE)
		return;

	UpdateStatelessSystems(deltaTime);
	switch (currentPrimaryGameState) {
	case PrimaryGameState::MAIN_MENU:
		primaryHUDRef->Update(deltaTime);
		break;
	case PrimaryGameState::OPTIONS_MENU:
		primaryHUDRef->Update(deltaTime);
		break;
	case PrimaryGameState::LEVEL_SELECT_MENU:
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
	//Update custom game systems
}


//Game State
bool APrimaryGameMode::StartGame(const ULevelSelectEntrySpec& spec) noexcept {
	if (gameStarted)
		return false;

	if (launchInDebugMode) {
		SetupPrePlayingState();
		SetupPlayingState();

		primaryPlayerControllerRef->SetControllerInputMode(ControllerInputMode::GAMEPLAY);
		primaryHUDRef->ClearViewport(); //No transition for now
	}
	else {
		levelManagementRef->LoadLevel(spec.key, [this]() {
			SetupPrePlayingState();
			SetupPlayingState();
			primaryPlayerControllerRef->SetControllerInputMode(ControllerInputMode::GAMEPLAY);
			primaryHUDRef->ClearViewport(); //No transition for now
			gameStarted = true;
		});
	}

	return true;
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

	SetupApplicationStartState();
	gameStarted = false;
}
void APrimaryGameMode::QuitGame() noexcept {
	UKismetSystemLibrary::QuitGame(GetWorld(), primaryPlayerControllerRef, EQuitPreference::Quit, false);
}

void APrimaryGameMode::PauseGame() noexcept {
	if (gamePaused || !gameStarted)
		return;

	gamePaused = UGameplayStatics::SetGamePaused(this, true);
	if (gamePaused) {
		primaryPlayerRef->SetPlayerHUDState(false);
		primaryHUDRef->SetMenuState(MenuState::PAUSE_MENU);
		primaryPlayerControllerRef->SetControllerInputMode(ControllerInputMode::PAUSED);
	}
}
void APrimaryGameMode::UnpauseGame() noexcept {
	if (!gamePaused || !gameStarted)
		return;

	gamePaused = UGameplayStatics::SetGamePaused(this, false);
	if (!gamePaused) {
		primaryPlayerRef->SetPlayerHUDState(true);
		primaryHUDRef->ClearViewport();
		primaryPlayerControllerRef->SetControllerInputMode(ControllerInputMode::GAMEPLAY);
	}
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
}
void APrimaryGameMode::SetupDependencies() noexcept {
	//Inject any dependencies required by any system.

	primaryPlayerRef->SetPrimaryGameModeReference(*this);
	primaryHUDRef->SetupDependencies(*this, *primaryPlayerRef, *primaryPlayerControllerRef);
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