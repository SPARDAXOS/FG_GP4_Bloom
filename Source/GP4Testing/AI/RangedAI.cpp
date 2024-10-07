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
				float ShootAngle = atan2(ShootDirection.Y, ShootDirection.X);
				ShootAngle = UKismetMathLibrary::RadiansToDegrees(ShootAngle) - 180;
				FRotator ShootRotator = FRotator::ZeroRotator;
				ShootRotator.Yaw = ShootAngle;
				
				ARangedAIBullet* SpawnedBullet = GetWorld()->SpawnActor<ARangedAIBullet>(Bullet, GetActorLocation(), ShootRotator);
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
