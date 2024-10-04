// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAI.h"
#include "Engine/DamageEvents.h"
#include "GP4Testing/PlayerSystems/PlayerHealthSystem.h"
#include "GP4Testing/Systems/PrimaryPlayer.h"

void AMeleeAI::Attack()
{
	if(bCanAttack)
	{
		Super::Attack();
		FHitResult Hit = GetHitDetectionResult();
		if (Hit.bBlockingHit)
		{
			if(Hit.GetActor()->ActorHasTag("Player")) // CHECK IF ENGINE TRACE HAS BEEN ADDED IF THIS DOESN'T FUNCTION
				{
				const FDamageEvent event;
				APrimaryPlayer* HitPlayer = Cast<APrimaryPlayer>(Hit.GetActor());
				HitPlayer->GetPlayerHealthSystem().HealthComponent->TakeDamage(Damage);
				}
		}
	}
	
}

void AMeleeAI::ResetAttack()
{
	Super::ResetAttack();
}
