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
class UPrimaryPlayerHUD;
class UCameraComponent;
class USpringArmComponent;


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

	inline bool GetActiveState() const noexcept { return active; }

public:
	inline void SetPrimaryGameModeReference(APrimaryGameMode& gameMode) noexcept { primaryGameModeRef = &gameMode; }

	inline AWeaponManagementSystem& GetWeaponManagementSystem() noexcept { return *weaponManagementSystemRef; }
	inline APickupManagementSystem& GetPickupManagementSystem() noexcept { return *pickupManagementSystemRef; }
	inline APlayerMovementSystem& GetPlayerMovementSystem() noexcept { return *playerMovementSystemRef; }
	inline APlayerHealthSystem& GetPlayerHealthSystem() noexcept { return *playerHealthSystemRef; }

	UFUNCTION(BlueprintCallable)
	AWeaponManagementSystem* GetWeaponManagementSystemPointer();

	inline UCameraComponent* GetCamera() noexcept { return cameraComponent; }
	inline UPrimaryPlayerHUD* GetPrimaryPlayerHUD() noexcept { return primaryPlayerHUDRef; }
	inline APrimaryGameMode* GetPrimaryGameMode() noexcept { return primaryGameModeRef; }

	inline FVector GetInitialCameraPosition() const noexcept { return cameraInitialTransform.GetLocation(); }


public: //Add Callbacks For Input Here!
	void HandleMovementInput(FVector2D axis) noexcept;
	void HandleLookInput(FVector2D axis) noexcept;
	void HandleJumpInput() noexcept;
	void HandleDashInput() noexcept;
	void HandleSlideInput() noexcept;
	void HandleShootInput(bool& input) noexcept;
	void HandleMeleeInput() noexcept;
	void HandlePauseInput() noexcept;
	void HandleReloadInput() noexcept;
	void HandleSwitchNextWeaponInput() noexcept;
	void HandleSwitchPreviousWeaponInput() noexcept;
	void HandleWeaponSlot1Input() noexcept;
	void HandleWeaponSlot2Input() noexcept;
	void HandleWeaponSlot3Input() noexcept;


private:
	void CreatePlayerSystems();
	void SetupPlayerSystemsDependencies() noexcept;
	void InitPlayerSystems() noexcept;
	void StartPlayerSystems() noexcept;
	void SetupCamera() noexcept;
	void SetupPrimaryPlayerHUD() noexcept;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> cameraComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> springArm = nullptr;

private: //Player  Systems
	UPROPERTY(EditDefaultsOnly, Category = "Player|Systems", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeaponManagementSystem> weaponManagementSystemAsset = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Player|Systems", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APickupManagementSystem> pickupManagementSystemAsset = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Player|Systems", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APlayerMovementSystem> playerMovementSystemAsset = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Player|Systems", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APlayerHealthSystem> playerHealthSystemAsset = nullptr;

private: //HUD
	UPROPERTY(EditDefaultsOnly, Category = "Player|HUD", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UPrimaryPlayerHUD> primaryPlayerHUDClass = nullptr;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Player|Audio", meta = (AllowPrivateAccess = "true"))
	USoundBase* fallLandSound;

private: //Camera
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player|Camera", meta = (AllowPrivateAccess = "true"))
	FTransform cameraInitialTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player|Camera", meta = (AllowPrivateAccess = "true"))
	float springArmLength;

private: //Melee
	UPROPERTY(EditDefaultsOnly)
	float MeleeRange = 250;

	UPROPERTY(EditDefaultsOnly)
	float MeleeCooldown = 1.f;

	bool bCanMelee = true;
	FTimerHandle TimerHandle;
	void ResetMelee();

public: //Camera shake - Explanation of UCameraShakeBase: https://dev.epicgames.com/documentation/en-us/unreal-engine/camera-shakes-in-unreal-engine
	APlayerCameraManager* CameraManager = nullptr;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> HitShake = nullptr;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> LandShake = nullptr;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> ShootingShakeRifle = nullptr;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> ShootingShakeShotgun = nullptr;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> RunningShake = nullptr;
	
	void ShakeCamera(TSubclassOf<UCameraShakeBase> CameraShakeBase, float Scale);
	inline void StopShakeCamera() { CameraManager->StopAllCameraShakes(); }

	UFUNCTION()
	void HandleHitShake();

	float StartJumpDistance;
	float DistanceFallen;
	void OnJumped_Implementation() override;
	void Landed(const FHitResult& Hit) override;
	UPROPERTY(EditDefaultsOnly)
	float MaxFallHeight = 1400.f;
	bool bIsGrounded = true;

	void HandleShootShakeRifle();
	void HandleShootShakeShotgun();

	
	void HandleRunningShake();
	UPROPERTY(EditDefaultsOnly)
	float MaxRunSpeed = 400.f;

	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Player|Debugging", meta = (AllowPrivateAccess = "true"))
	bool active = false;

private:
	TObjectPtr<AWeaponManagementSystem> weaponManagementSystemRef = nullptr;
	TObjectPtr<APickupManagementSystem> pickupManagementSystemRef = nullptr;
	TObjectPtr<APlayerMovementSystem> playerMovementSystemRef = nullptr;
	TObjectPtr<APlayerHealthSystem> playerHealthSystemRef = nullptr;
	TObjectPtr<UPrimaryPlayerHUD> primaryPlayerHUDRef = nullptr;

private:
	APrimaryGameMode* primaryGameModeRef = nullptr;
};
