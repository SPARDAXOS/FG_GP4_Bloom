#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Entity.h"

#include "GP4Testing/Utility/Timer.h"
#include "PrimaryPlayer.generated.h"


class APrimaryGameMode;
class AWeaponManagementSystem;
class APickupManagementSystem;
class APlayerMovementSystem;
class APlayerHealthSystem;


UCLASS(Abstract)
class GP4TESTING_API APrimaryPlayer : public ACharacter, public Entity {
	GENERATED_BODY()

public:
	APrimaryPlayer();

public:
	virtual void Init() override;
	virtual void Start() override;
	virtual void Update(float deltaTime) override;


public:
	void SetPlayerState(bool state) noexcept;
	void SetPlayerHUDState(bool state) noexcept;
	void SetupStartingState() noexcept;

public:
	inline void SetPrimaryGameModeReference(APrimaryGameMode& gameMode) noexcept { primaryGameModeRef = &gameMode; }

	inline AWeaponManagementSystem& GetWeaponManagementSystem() noexcept { return *weaponManagementSystemRef; }
	inline APickupManagementSystem& GetPickupManagementSystem() noexcept { return *pickupManagementSystemRef; }
	inline APlayerMovementSystem& GetPlayerMovementSystem() noexcept { return *playerMovementSystemRef; }
	inline APlayerHealthSystem& GetPlayerHealthSystem() noexcept { return *playerHealthSystemRef; }

public: //Add Callbacks For Input Here!
	void HandleMovementInput(FVector2D axis) noexcept;
	void HandleLookInput(FVector2D axis) noexcept;
	void HandleJumpInput() noexcept;
	void HandleShootInput(bool& input) noexcept;
	void HandlePauseInput() noexcept;


private:
	void CreatePlayerSystems();
	void SetupPlayerSystemsDependencies() noexcept;
	void InitPlayerSystems() noexcept;
	void StartPlayerSystems() noexcept;

private: //Player  Systems
	UPROPERTY(EditDefaultsOnly, Category = "Player|Systems", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeaponManagementSystem> weaponManagementSystemAsset = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Player|Systems", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APickupManagementSystem> pickupManagementSystemAsset = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Player|Systems", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APlayerMovementSystem> playerMovementSystemAsset = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Player|Systems", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APlayerHealthSystem> playerHealthSystemAsset = nullptr;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Player|Debugging", meta = (AllowPrivateAccess = "true"))
	bool active = false;

private:
	TObjectPtr<AWeaponManagementSystem> weaponManagementSystemRef = nullptr;
	TObjectPtr<APickupManagementSystem> pickupManagementSystemRef = nullptr;
	TObjectPtr<APlayerMovementSystem> playerMovementSystemRef = nullptr;
	TObjectPtr<APlayerHealthSystem> playerHealthSystemRef = nullptr;

private:
	APrimaryGameMode* primaryGameModeRef = nullptr;
};
