// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedAI.h"

#include "RangedAIBullet.h"
#include "Engine/DamageEvents.h"
#include "GP4Testing/Systems/PrimaryPlayer.h"
#include "GP4Testing/Utility/Debugging.h"
#include "GP4Testing/VFXEntities/TriggerVFX.h"
#include "Kismet/KismetMathLibrary.h"
#include <Kismet/GameplayStatics.h>

ARangedAI::ARangedAI()
{
	Muzzle = CreateDefaultSubobject<USceneComponent>("Muzzle");
	Muzzle->SetupAttachment(GetRootComponent());
	Hitbox = CreateDefaultSubobject<USphereComponent>("Hitbox");
	Hitbox->SetupAttachment(GetRootComponent());
	DeathVFXSpawnLoc = CreateDefaultSubobject<USceneComponent>("DeathVFXSpawnLoc");
	DeathVFXSpawnLoc->SetupAttachment(GetRootComponent());
}

void ARangedAI::EnemyDeath()
{
	SetEnemyState(false);
}

void ARangedAI::Die()
{
	Super::Die();

	if (DeathSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}

	SetEnemyState(false);
	DeathVFX->SetActorLocation(GetActorLocation());
	DeathVFX->Activate();
}


void ARangedAI::BeginPlay()
{
	Super::BeginPlay();
	DeathVFX = GetWorld()->SpawnActor<ATriggerVFX>(TriggerVfx, DeathVFXSpawnLoc->GetComponentLocation(), GetActorRotation());
	FOnVFXFinishedSignature callback;
	if (DeathVFX)
	{
		callback.BindUFunction(this, FName("EnemyDeath"));
		DeathVFX->SetupOnFinishedCallback(callback);
	}
}

void ARangedAI::Attack()
{
	if(bCanAttack)
	{
		Super::Attack();
		FHitResult Hit = GetHitDetectionResult(Muzzle->GetComponentLocation());
		FVector ActorLoc = FVector(GetActorLocation().X, GetActorLocation().Y, 0);
		FVector PlayerLoc = FVector(Player->GetActorLocation().X, Player->GetActorLocation().Y, 0);
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(ActorLoc, PlayerLoc));

		if (Hit.bBlockingHit)
		{
			if (Hit.GetActor()->ActorHasTag("Player"))
			{
				const FDamageEvent event;
				FVector ShootDirection = Muzzle->GetComponentLocation() - Player->GetActorLocation();
				ShootDirection.Normalize();
				FRotator ShootRotation = UKismetMathLibrary::MakeRotFromX(ShootDirection);

				float ShootAngle = atan2(ShootDirection.Y, ShootDirection.X);
				FRotator ShootRotator = FRotator::ZeroRotator;
				float ShootYaw = UKismetMathLibrary::RadiansToDegrees(ShootAngle) - 180;
				ShootRotator.Yaw = ShootYaw;
				
				ARangedAIBullet* SpawnedBullet = GetWorld()->SpawnActor<ARangedAIBullet>(Bullet, Muzzle->GetComponentLocation(), FRotator(-ShootRotation.Pitch, ShootRotator.Yaw, 0.f));
				if(SpawnedBullet)
				{
					SpawnedBullet->SetDamage(Damage);
				}
			}
		}
	}
}

void ARangedAI::ResetAttack()
{
	Super::ResetAttack();
}

void ARangedAI::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	Blackboard->SetValueAsBool("bCanSeePlayer", bCanSeePlayer());
}

bool ARangedAI::bCanSeePlayer()
{
	FCollisionQueryParams TraceParams;
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, Muzzle->GetComponentLocation(), Player->GetActorLocation(), ECC_Visibility, TraceParams);

	if (Hit.GetActor())
	{
		APrimaryPlayer* HitPlayer = Cast<APrimaryPlayer>(Hit.GetActor());
		if(HitPlayer)
		{
			float distance = FVector::Distance(GetActorLocation(), HitPlayer->GetActorLocation());
			if(distance <= VisionRange)
			{
				return true;
			}
		}
	}
	return false;
}
