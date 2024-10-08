#include "GunComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/DamageEvents.h"
#include "../Systems/PrimaryPlayerController.h"
#include "../Systems/PrimaryPlayer.h"
#include <GP4Testing/Components/HealthComponent.h>
#include "../AI/EnemyAIBase.h"
#include "GP4Testing/PlayerSystems/WeaponManagementSystem.h"

UGunComponent::UGunComponent()
{
	GL_MuzzleOffset = FVector(100, 0, 10);
}

void UGunComponent::Fire()
{
	bFiredWeapon = true;

	UE_LOG(LogTemp, Warning, TEXT("Current Ammo: %f"), Ammo);
	UE_LOG(LogTemp, Warning, TEXT("Current Magazine: %f"), Magazine);

	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	if (Magazine > 0)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			Magazine--;

			FHitResult Hit;

			APrimaryPlayerController* PlayerController = Cast<APrimaryPlayerController>(Character->GetController());

			FVector ViewOrigin;
			FRotator ViewRotation;

			PlayerController->GetPlayerViewPoint(ViewOrigin, ViewRotation);

			FVector ViewForward = ViewRotation.Quaternion().GetForwardVector();

			if (TypeOfWeapon == WeaponType::GRENADE_LAUNCHER)
			{
				const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
				const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(GL_MuzzleOffset);

				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				World->SpawnActor<AExplosiveProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
			else
			{
				for (int i = 0; i < BulletsPerShot; i++)
				{
					World->LineTraceSingleByChannel(
						Hit,
						ViewOrigin, GetBulletSpread(ViewOrigin, ViewForward),
						ECollisionChannel::ECC_GameTraceChannel3
					);

					DrawDebugLine(World, ViewOrigin, GetBulletSpread(ViewOrigin, ViewForward), FColor::Red, false, 5.0f, 0, 1.0f);
				}
			}

			// Do damage if enemy is hit
			if (Hit.bBlockingHit)
			{
				AEnemyAIBase* Enemy = Cast<AEnemyAIBase>(Hit.GetActor());

				if (Enemy->HealthComponent != nullptr)
				{
					UE_LOG(LogTemp, Warning, TEXT("Enemy health: %f"), Enemy->HealthComponent->CurrentHealth);
					Enemy->HealthComponent->TakeDamage(WeaponDamage);
				}
			}
		}

		// Play fire sound when shooting
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
		}

		// Play fire animation when shooting
		if (FireAnimation != nullptr)
		{
			USkeletalMeshComponent* Mesh = Character->FindComponentByClass<USkeletalMeshComponent>();
			UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
			if (AnimInstance != nullptr)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}
	}
}

FVector UGunComponent::GetBulletSpread(FVector ViewOrigin, FVector ViewForward)
{
	FVector Target = ViewOrigin + ViewForward * LineTraceDistance;

	Target = FVector(
		Target.X = FMath::RandRange(-BulletSpread, BulletSpread),
		Target.Y = FMath::RandRange(-BulletSpread, BulletSpread),
		Target.Z = FMath::RandRange(-BulletSpread, BulletSpread)
	);

	FVector Direction = (ViewOrigin + ViewForward * LineTraceDistance) + Target;

	return Direction;
}

void UGunComponent::Reload()
{
	if (Magazine < MaxMagazine && Ammo > 0)
	{
		for (int i = 0; Magazine < MaxMagazine; i++)
		{
			Magazine++;
			Ammo--;
		}

		UE_LOG(LogTemp, Warning, TEXT("Current Ammo: %f"), Ammo);
		UE_LOG(LogTemp, Warning, TEXT("Current Magazine: %f"), Magazine);
	}
}

void UGunComponent::StartFire()
{
	if (TypeOfWeapon == WeaponType::MACHINE_GUN)
	{
		Fire();
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UGunComponent::Fire, AutoFireRate, true);
	}
	if (TypeOfWeapon == WeaponType::SHOTGUN || TypeOfWeapon == WeaponType::GRENADE_LAUNCHER)
	{
		if (!bFiredWeapon)
		{
			Fire();
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UGunComponent::StopFire, NonAutoFireRate, false);
		}
	}
}

void UGunComponent::StopFire()
{
	bFiredWeapon = false;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

// Attach weapon the the player
void UGunComponent::AttachWeapon(APrimaryPlayer* TargetCharacter)
{
	Character = TargetCharacter;

	if (Character == nullptr)
	{
		return;
	}

	if (TypeOfWeapon == WeaponType::NONE)
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon type has not been set!"));
		return;
	}

	bool HasWeapon = Character->GetWeaponManagementSystem().GetHasWeapon();

	if (!HasWeapon)
	{
		Character->GetWeaponManagementSystem().SetHasWeapon(true);

		Magazine = MaxMagazine;
		Ammo = MaxAmmo;

		// Attach to player mesh
		USkeletalMeshComponent* Mesh = Character->FindComponentByClass<USkeletalMeshComponent>();
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		AttachToComponent(Mesh, AttachmentRules, FName(TEXT("GripPoint")));

		if (APrimaryPlayerController* PlayerController = Cast<APrimaryPlayerController>(Character->GetController()))
		{
			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				if (TypeOfWeapon == WeaponType::MACHINE_GUN)
				{
					EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &UGunComponent::StartFire);
					EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &UGunComponent::StopFire);
				}
				if (TypeOfWeapon == WeaponType::SHOTGUN || TypeOfWeapon == WeaponType::GRENADE_LAUNCHER)
				{
					EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &UGunComponent::StartFire);
				}
				EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &UGunComponent::Reload);
			}
		}
	}
}