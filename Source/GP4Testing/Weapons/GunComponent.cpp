#include "GunComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/DamageEvents.h"
#include "NiagaraComponent.h"
#include "../Systems/PrimaryPlayerController.h"
#include "../Systems/PrimaryPlayer.h"
#include <GP4Testing/Components/HealthComponent.h>
#include "../AI/EnemyAIBase.h"
#include "GP4Testing/PlayerSystems/WeaponManagementSystem.h"
#include "GP4Testing/Utility/Debugging.h"

AGunComponent::AGunComponent()
{
	GL_MuzzleOffset = FVector(0, 60, 10);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	RootComponent = WeaponMesh;

	VFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFX"));
	VFX->SetupAttachment(WeaponMesh);

	VFX2 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFX2"));
	VFX2->SetupAttachment(WeaponMesh);
	
}

void AGunComponent::Fire()
{
	bFiredWeapon = true;

	UE_LOG(LogTemp, Warning, TEXT("Current Ammo: %f"), Ammo);
	UE_LOG(LogTemp, Warning, TEXT("Current Magazine: %f"), Magazine);

	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	if (Magazine > 0 && !bReloading)
	{
		VFX->Activate(true);

		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			Magazine--;

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
					Debugging::PrintString("Started fire");
					FHitResult Hit;
					World->LineTraceSingleByChannel(
						Hit,
						ViewOrigin, GetBulletSpread(ViewOrigin, ViewForward),
						ECollisionChannel::ECC_GameTraceChannel3
					);
					// Do damage if enemy is hit
					if (Hit.bBlockingHit)
					{
						Debugging::PrintString("Hit an object");
						AEnemyAIBase* Enemy = Cast<AEnemyAIBase>(Hit.GetActor());
						if (Enemy != nullptr)
						{
							if (Enemy->HealthComponent != nullptr)
							{
								Debugging::PrintString("Damage");
								UE_LOG(LogTemp, Warning, TEXT("Enemy health: %f"), Enemy->HealthComponent->CurrentHealth);
								Enemy->HealthComponent->TakeDamage(WeaponDamage);
							}
						}

						VFX2->Activate(true);
					}
					DrawDebugLine(World, ViewOrigin, GetBulletSpread(ViewOrigin, ViewForward), FColor::Red, false, 4.f, 0, 1.0f);
				}
			}

			// Camera shake when shooting
			if (TypeOfWeapon == WeaponType::MACHINE_GUN)
			{
				Character->HandleShootShakeRifle();
			}
			if (TypeOfWeapon == WeaponType::SHOTGUN)
			{
				Character->HandleShootShakeShotgun();
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
	else if(Magazine <= 0 && !bReloading)
	{
		Reload();
	}
}

void AGunComponent::ReloadTimer()
{
	bReloading = false;
	for (int i = 0; Magazine < MaxMagazine; i++)
	{
		if (Ammo == 0)
		{
			return;
		}
		Magazine++;
		Ammo--;
	}

	UE_LOG(LogTemp, Warning, TEXT("Current Ammo: %f"), Ammo);
	UE_LOG(LogTemp, Warning, TEXT("Current Magazine: %f"), Magazine);
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
}

FVector AGunComponent::GetBulletSpread(FVector ViewOrigin, FVector ViewForward)
{
	FVector Target = ViewOrigin + ViewForward * LineTraceDistance;

	Target = FVector(
		Target.X = FMath::RandRange(-BulletSpreadX, BulletSpreadX),
		Target.Y = FMath::RandRange(-BulletSpreadY, BulletSpreadY),
		Target.Z = FMath::RandRange(-BulletSpreadZ, BulletSpreadZ) 
	);

	FVector Direction = (ViewOrigin + ViewForward * LineTraceDistance) + Target;

	return Direction;
}

void AGunComponent::Reload()
{
	if (Magazine < MaxMagazine && Ammo > 0)
	{
		bReloading = true;
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AGunComponent::ReloadTimer, ReloadLength, false);
		if (ReloadAnimation != nullptr)
		{
			USkeletalMeshComponent* Mesh = Character->FindComponentByClass<USkeletalMeshComponent>();
			UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
			if (AnimInstance != nullptr)
			{
				AnimInstance->Montage_Play(ReloadAnimation, 1.f);
			}
		}
	}
}

void AGunComponent::StartFire()
{

	if (TypeOfWeapon == WeaponType::MACHINE_GUN)
	{
		Fire();
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AGunComponent::Fire, AutoFireRate, true);
	}
	if (TypeOfWeapon == WeaponType::SHOTGUN || TypeOfWeapon == WeaponType::GRENADE_LAUNCHER)
	{
		if (!bFiredWeapon)
		{
			Fire();
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AGunComponent::StopFire, NonAutoFireRate, false);
		}
	}
}

void AGunComponent::StopFire()
{
	bFiredWeapon = false;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void AGunComponent::ClearTimers()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
}

WeaponType AGunComponent::GetWeaponType()
{
	return TypeOfWeapon;
}

// Attach weapon the the player
void AGunComponent::AttachWeapon(APrimaryPlayer* TargetCharacter)
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

	TMap<WeaponType, AGunComponent*> AcquiredWeapons = Character->GetWeaponManagementSystem().GetAcquiredWeapons();
	bool HasWeapon = false;
	for (auto Weapon : AcquiredWeapons)
	{
		if (Weapon.Key == TypeOfWeapon)
		{
			HasWeapon = true;
		}
	}

	if (!HasWeapon)
	{
		Character->GetWeaponManagementSystem().AcquireWeapon(TypeOfWeapon, this);

		Magazine = MaxMagazine;
		Ammo = MaxAmmo;

		// Attach to player mesh
		USkeletalMeshComponent* Mesh = Character->FindComponentByClass<USkeletalMeshComponent>();
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		AttachToComponent(Mesh, AttachmentRules, FName(TEXT("GripPoint")));
	}
}

void AGunComponent::EndPlay()
{
	bReloading = false;
	StopFire();
	ClearTimers();
	USkeletalMeshComponent* Mesh = Character->FindComponentByClass<USkeletalMeshComponent>();
	UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		AnimInstance->StopAllMontages(0);
	}
}