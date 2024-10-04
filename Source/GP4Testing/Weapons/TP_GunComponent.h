#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "TP_GunComponent.generated.h"

class APrimaryPlayer;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GP4TESTING_API UTP_GunComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
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
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Reload();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StartAutomaticFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StopAutomaticFire();

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
	float LineTraceDistance = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
	float BulletSpread = 5.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
	bool bAutomatic = false;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
	float AutomaticFireRate = 0.15f;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
	float BulletsPerShot = 1.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
	float SingleFireDelay = 1.5f;

	FVector GetBulletSpread(FVector ViewOrigin, FVector ViewForward);

	FTimerHandle TimerHandle;

	void FireDelay();

	void StopFireDelay();

	bool bFiredWeapon = false;
};
