#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include <GP4Testing/PlayerSystems/WeaponTypes.h>
#include "ExplosiveProjectile.h"
#include "GunComponent.generated.h"

class APrimaryPlayer;
class UNiagaraComponent;
class AWeaponSpawner;
class AImpactVFX;

UCLASS()
class GP4TESTING_API AGunComponent : public AActor
{
	GENERATED_BODY()

public:
	AGunComponent();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* ReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReloadAction;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AttachWeapon(APrimaryPlayer* TargetCharacter);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Reload();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StopFire();

	UFUNCTION()
	void ClearTimers();

	inline void RegisterPickupSpawner(AWeaponSpawner& spawner) noexcept { RegisteredWeaponSpawner = &spawner; }

	UFUNCTION(BlueprintCallable)
	WeaponType GetWeaponType();

	UPROPERTY(EditDefaultsOnly, Category = "Grenade Launcher", meta = (EditCondition = "TypeOfWeapon == WeaponType::GRENADE_LAUNCHER", EditConditionHides))
	TSubclassOf<AExplosiveProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Grenade Launcher", meta = (EditCondition = "TypeOfWeapon == WeaponType::GRENADE_LAUNCHER", EditConditionHides))
	FVector GL_MuzzleOffset;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings");
	WeaponType TypeOfWeapon = WeaponType::NONE;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
	int MaxMagazine;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
	int32 MaxAmmo;

	int32 Magazine;

	int32 Ammo;

public:
	UPROPERTY(Editanywhere)
	UNiagaraComponent* VFX;

	void Fire();

	FHitResult Hit;

private:
	APrimaryPlayer* Character;
	AWeaponSpawner* RegisteredWeaponSpawner = nullptr;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings", meta = (EditCondition = "TypeOfWeapon == WeaponType::MACHINE_GUN || TypeOfWeapon == WeaponType::SHOTGUN", EditConditionHides))
	float WeaponDamage = 20;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings", meta = (EditCondition = "TypeOfWeapon == WeaponType::MACHINE_GUN || TypeOfWeapon == WeaponType::SHOTGUN", EditConditionHides))
	float LineTraceDistance = 20000.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings", meta = (EditCondition = "TypeOfWeapon == WeaponType::MACHINE_GUN || TypeOfWeapon == WeaponType::SHOTGUN", EditConditionHides))
	float BulletSpreadX = 5.f;
	UPROPERTY(EditAnywhere, Category = "Weapon Settings", meta = (EditCondition = "TypeOfWeapon == WeaponType::MACHINE_GUN || TypeOfWeapon == WeaponType::SHOTGUN", EditConditionHides))
	float BulletSpreadY = 5.f;
	UPROPERTY(EditAnywhere, Category = "Weapon Settings", meta = (EditCondition = "TypeOfWeapon == WeaponType::MACHINE_GUN || TypeOfWeapon == WeaponType::SHOTGUN", EditConditionHides))
	float BulletSpreadZ = 5.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings", meta = (EditCondition = "TypeOfWeapon == WeaponType::MACHINE_GUN || TypeOfWeapon == WeaponType::SHOTGUN", EditConditionHides))
	float BulletsPerShot = 1.f;

	UPROPERTY(EditAnywhere, Category = "Non Auto Weapon Settings", meta = (EditCondition = "TypeOfWeapon == WeaponType::SHOTGUN", EditConditionHides))
	float NonAutoFireRate = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Auto Weapon Settings", meta = (EditCondition = "TypeOfWeapon == WeaponType::MACHINE_GUN", EditConditionHides))
	float AutoFireRate = 0.15f;

	UPROPERTY(EditAnywhere, Category = "VFX")
	TSubclassOf<AImpactVFX> ImpactVFXClass = nullptr;

	FVector GetBulletSpread(FVector ViewOrigin, FVector ViewForward);

	FTimerHandle TimerHandle;
	FTimerHandle ReloadTimerHandle;
	FTimerHandle VFXTimerHandle;
	

	bool bFiredWeapon = false;

private:
	void ReloadTimer();
	TObjectPtr<AImpactVFX> impactVFX;

public:
	UPROPERTY(EditAnywhere, Category = "Non Auto Weapon Settings", meta = (EditCondition = "TypeOfWeapon == WeaponType::SHOTGUN", EditConditionHides))
	float ReloadLength = 0;

	bool bReloading = false;

public:
	void EndPlay();
};
