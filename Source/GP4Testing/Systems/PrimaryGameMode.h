#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "PrimaryGameMode.generated.h"



UENUM(BlueprintType)
enum class PrimaryGameState : uint8 {
	NONE = 0,
	MAIN_MENU,
	OPTIONS_MENU,
	LEVEL_SELECT_MENU,
	PLAYING,
	PAUSED
};


class APrimaryPlayer;
class APrimaryPlayerController;
class APrimaryHUD;

class ALevelManagement;
class AEnemyManagementSystem;
class AWaveManager;
class ULevelSelectEntrySpec;


UCLASS(abstract)
class GP4TESTING_API APrimaryGameMode : public AGameModeBase {
	GENERATED_BODY()

public:
	APrimaryGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void StartPlay() override;

public:
	inline APrimaryPlayer* GetPrimaryPlayer() const noexcept { return primaryPlayerRef; }
	inline APrimaryPlayerController* GetPrimaryPlayerController() const noexcept { return primaryPlayerControllerRef; }
	inline APrimaryHUD* GetPrimaryHUD() const noexcept { return primaryHUDRef; }

	static inline ALevelManagement* GetLevelManagement() noexcept { return levelManagementRef; }

	inline AEnemyManagementSystem* GetEnemyManagementSystem() noexcept { return enemyManagementSystemRef; }
	inline AWaveManager* GetWaveManager() noexcept { return waveManagerRef; }

public:
	bool StartGame(const ULevelSelectEntrySpec& spec) noexcept;
	void EndGame() noexcept;
	void QuitGame() noexcept;

	void PauseGame() noexcept;
	void UnpauseGame() noexcept;
	
public:
	inline bool GetGameStarted() const noexcept { return gameStarted; }
	inline bool GetGamePaused() const noexcept { return gamePaused; }

public:

private:
	void UpdateGame(float deltaTime) noexcept;

private:
	void UpdateStatelessSystems(float deltaTime);
	void UpdatePlayingStateSystems(float deltaTime);
	
private:
	void SetupApplicationStartState() noexcept;
	void SetupPrePlayingState() noexcept;
	void SetupPlayingState() noexcept;

private:
	void InitMainSystems();
	void BroadcastInit();
	void BroadcastStart();
	void CreateSystems() noexcept;
	void SetupDependencies() noexcept;
	void CacheMainSystemsReferences() noexcept;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Systems|CustomSystems", meta = (AllowPrivateAcces = "true"))
	TSubclassOf<ALevelManagement> levelManagementClass;

	UPROPERTY(EditDefaultsOnly, Category = "Systems|CustomSystems", meta = (AllowPrivateAcces = "true"))
	TSubclassOf<AEnemyManagementSystem> enemyManagementSystemClass;

	UPROPERTY(EditDefaultsOnly, Category = "Systems|CustomSystems", meta = (AllowPrivateAcces = "true"))
	TSubclassOf<AWaveManager> waveManagerClass;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Systems|LevelManagement", meta = (AllowPrivateAcces = "true"))
	TArray<FName> loadedLevelsOnGameStart;

private:
	UPROPERTY(VisibleAnywhere, Category = "Debugging", meta = (AllowPrivateAcces = "true"))
	PrimaryGameState currentPrimaryGameState = PrimaryGameState::NONE;

	UPROPERTY(VisibleAnywhere, Category = "Debugging", meta = (AllowPrivateAccess = "true"))
	bool simulationMode = false;

	UPROPERTY(VisibleAnywhere, Category = "Debugging", meta = (AllowPrivateAcces = "true"))
	bool gameStarted = false;

	UPROPERTY(VisibleAnywhere, Category = "Debugging", meta = (AllowPrivateAcces = "true"))
	bool gamePaused = false;

	UPROPERTY(EditDefaultsOnly, Category = "Debugging", meta = (AllowPrivateAcces = "true"))
	bool launchInDebugMode = false;

	UPROPERTY(VisibleAnywhere, Category = "Debugging", meta = (AllowPrivateAcces = "true"))
	FName loadedLevelKey = "None";

private:
	TObjectPtr<APrimaryPlayer> primaryPlayerRef = nullptr;
	TObjectPtr<APrimaryPlayerController> primaryPlayerControllerRef = nullptr;
	TObjectPtr<APrimaryHUD> primaryHUDRef = nullptr;

private:
	inline static TObjectPtr<ALevelManagement> levelManagementRef = nullptr;
	TObjectPtr<AEnemyManagementSystem> enemyManagementSystemRef = nullptr;
	TObjectPtr<AWaveManager> waveManagerRef = nullptr;

};
