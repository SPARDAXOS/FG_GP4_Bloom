#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include <GP4Testing/PlayerSystems/WeaponTypes.h>
#include "ExplosiveProjectile.h"
#include "GunComponent.generated.h"

class APrimaryPlayer;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GP4TESTING_API UGunComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	UGunComponent();

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


	UPROPERTY(EditDefaultsOnly, Category = "Grenade Launcher")
	TSubclassOf<AExplosiveProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Grenade Launcher")
	FVector GL_MuzzleOffset;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings");
	WeaponType TypeOfWeapon = WeaponType::NONE;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
	float MaxMagazine;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
	float MaxAmmo;

	float Magazine;

	float Ammo;

private:
	APrimaryPlayer* Character;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
	float WeaponDamage = 20;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
	float LineTraceDistance = 20000.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
	float BulletSpread = 5.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
	float BulletsPerShot = 1.f;

	UPROPERTY(EditAnywhere, Category = "Non Auto Weapon Settings")
	float NonAutoFireRate = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Auto Weapon Settings")
	float AutoFireRate = 0.15f;

	FVector GetBulletSpread(FVector ViewOrigin, FVector ViewForward);

	FTimerHandle TimerHandle;

	void Fire();

	bool bFiredWeapon = false;
};
