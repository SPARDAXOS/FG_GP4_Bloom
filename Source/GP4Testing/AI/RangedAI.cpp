// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedAI.h"

#include "RangedAIBullet.h"
#include "Engine/DamageEvents.h"
#include "GP4Testing/Systems/PrimaryPlayer.h"
#include "GP4Testing/Utility/Debugging.h"
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

void ARangedAI::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	Blackboard->SetValueAsBool("bCanSeePlayer", bCanSeePlayer());
}

bool ARangedAI::bCanSeePlayer()
{
	FCollisionQueryParams TraceParams;
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), Player->GetActorLocation(), ECC_Visibility, TraceParams);

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
