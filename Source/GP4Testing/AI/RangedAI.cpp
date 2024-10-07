// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedAI.h"

#include "RangedAIBullet.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetMathLibrary.h"

void ARangedAI::Attack()
{
	if(bCanAttack)
	{
		Super::Attack();
		FHitResult Hit = GetHitDetectionResult();

		if (Hit.bBlockingHit)
		{
			if (Hit.GetActor()->ActorHasTag("Player"))
			{
				const FDamageEvent event;
				FVector ShootDirection = GetActorLocation() - Player->GetActorLocation();
				ShootDirection.Normalize();
				FRotator ShootRotation = UKismetMathLibrary::MakeRotFromX(ShootDirection);

				float ShootAngle = atan2(ShootDirection.Y, ShootDirection.X);
				FRotator ShootRotator = FRotator::ZeroRotator;
				float ShootYaw = UKismetMathLibrary::RadiansToDegrees(ShootAngle) - 180;
				ShootRotator.Yaw = ShootYaw;
				
				ARangedAIBullet* SpawnedBullet = GetWorld()->SpawnActor<ARangedAIBullet>(Bullet, GetActorLocation(), FRotator(-ShootRotation.Pitch, ShootRotator.Yaw, 0.f));
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
