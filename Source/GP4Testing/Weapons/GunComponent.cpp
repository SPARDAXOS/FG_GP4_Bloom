#include "GunComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/DamageEvents.h"
#include "../Systems/PrimaryPlayerController.h"
#include "../Systems/PrimaryPlayer.h"
#include <GP4Testing/Components/HealthComponent.h>

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

			for (int i = 0; i < BulletsPerShot; i++)
			{
				World->LineTraceSingleByChannel(
					Hit,
					ViewOrigin, GetBulletSpread(ViewOrigin, ViewForward),
					ECollisionChannel::ECC_GameTraceChannel3
				);

				DrawDebugLine(World, ViewOrigin, GetBulletSpread(ViewOrigin, ViewForward), FColor::Red, false, 5.0f, 0, 1.0f);
			}

			// Do damage if enemy is hit
			if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
			{
				UHealthComponent* health = Cast<UHealthComponent>(Hit.GetActor());
				if (health != nullptr)
				{
					UE_LOG(LogTemp, Warning, TEXT("Enemy health: %f"), health->CurrentHealth);
					health->TakeDamage(WeaponDamage);
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

void UGunComponent::FireDelay()
{
	if (!bFiredWeapon)
	{
		Fire();
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UGunComponent::StopFireDelay, SingleFireDelay, false);
	}
}

void UGunComponent::StopFireDelay()
{
	bFiredWeapon = false;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
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

void UGunComponent::StartAutomaticFire()
{
	Fire();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UGunComponent::Fire, AutomaticFireRate, true);
}

void UGunComponent::StopAutomaticFire()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

// Attach weapon the the player
void UGunComponent::AttachWeapon(APrimaryPlayer* TargetCharacter)
{
	Character = TargetCharacter;

	Magazine = MaxMagazine;
	Ammo = MaxAmmo;

	if (Character == nullptr)
	{
		return;
	}

	// Attach to player mesh
	USkeletalMeshComponent* Mesh = Character->FindComponentByClass<USkeletalMeshComponent>();
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Mesh, AttachmentRules, FName(TEXT("GripPoint")));

	if (APrimaryPlayerController* PlayerController = Cast<APrimaryPlayerController>(Character->GetController()))
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			if (bAutomatic)
			{
				EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &UGunComponent::StartAutomaticFire);
				EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &UGunComponent::StopAutomaticFire);
			}
			else
			{
				EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &UGunComponent::FireDelay);
			}
			EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &UGunComponent::Reload);
		}
	}
}