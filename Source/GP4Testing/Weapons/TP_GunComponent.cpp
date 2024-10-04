#include "TP_GunComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/DamageEvents.h"
#include "../Systems/PrimaryPlayerController.h"
#include "../Systems/PrimaryPlayer.h"

void UTP_GunComponent::Fire()
{
	bFiredWeapon = true;
	//Hello
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
				if (Hit.GetActor() != Character)
				{
					
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

FVector UTP_GunComponent::GetBulletSpread(FVector ViewOrigin, FVector ViewForward)
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

void UTP_GunComponent::FireDelay()
{
	if (!bFiredWeapon)
	{
		Fire();
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTP_GunComponent::StopFireDelay, SingleFireDelay, false);
	}
}

void UTP_GunComponent::StopFireDelay()
{
	bFiredWeapon = false;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UTP_GunComponent::Reload()
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

void UTP_GunComponent::StartAutomaticFire()
{
	Fire();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTP_GunComponent::Fire, AutomaticFireRate, true);
}

void UTP_GunComponent::StopAutomaticFire()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

// Attach weapon the the player
void UTP_GunComponent::AttachWeapon(APrimaryPlayer* TargetCharacter)
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
				EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &UTP_GunComponent::StartAutomaticFire);
				EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &UTP_GunComponent::StopAutomaticFire);
			}
			else
			{
				EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &UTP_GunComponent::FireDelay);
			}
			EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &UTP_GunComponent::Reload);
		}
	}
}