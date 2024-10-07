// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedAI.h"

#include "RangedAIBullet.h"
#include "Engine/DamageEvents.h"

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
				ARangedAIBullet* SpawnedBullet = GetWorld()->SpawnActor<ARangedAIBullet>(Bullet, GetActorLocation(), GetActorRotation());
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
