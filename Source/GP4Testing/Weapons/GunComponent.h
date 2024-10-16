#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include <GP4Testing/PlayerSystems/WeaponTypes.h>
#include "ExplosiveProjectile.h"
#include "GunComponent.generated.h"

class APrimaryPlayer;
class UNiagaraComponent;

UCLASS()
class GP4TESTING_API AGunComponent : public AActor
{
	GENERATED_BODY()

public:
	AGunComponent();

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

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
	void ClearWeaponTimer();

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

private:
	APrimaryPlayer* Character;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings", meta = (EditCondition = "TypeOfWeapon == WeaponType::MACHINE_GUN || TypeOfWeapon == WeaponType::SHOTGUN", EditConditionHides))
	float WeaponDamage = 20;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings", meta = (EditCondition = "TypeOfWeapon == WeaponType::MACHINE_GUN || TypeOfWeapon == WeaponType::SHOTGUN", EditConditionHides))
	float LineTraceDistance = 20000.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings", meta = (EditCondition = "TypeOfWeapon == WeaponType::MACHINE_GUN || TypeOfWeapon == WeaponType::SHOTGUN", EditConditionHides))
	float BulletSpread = 5.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings", meta = (EditCondition = "TypeOfWeapon == WeaponType::MACHINE_GUN || TypeOfWeapon == WeaponType::SHOTGUN", EditConditionHides))
	float BulletsPerShot = 1.f;

	UPROPERTY(EditAnywhere, Category = "Non Auto Weapon Settings", meta = (EditCondition = "TypeOfWeapon == WeaponType::SHOTGUN", EditConditionHides))
	float NonAutoFireRate = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Auto Weapon Settings", meta = (EditCondition = "TypeOfWeapon == WeaponType::MACHINE_GUN", EditConditionHides))
	float AutoFireRate = 0.15f;

	FVector GetBulletSpread(FVector ViewOrigin, FVector ViewForward);

	FTimerHandle TimerHandle;

	void Fire();

	bool bFiredWeapon = false;
};
