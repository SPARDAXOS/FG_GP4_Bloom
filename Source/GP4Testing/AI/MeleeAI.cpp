// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAI.h"

#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GP4Testing/PlayerSystems/PlayerHealthSystem.h"
#include "GP4Testing/Systems/PrimaryPlayer.h"
#include "GP4Testing/Utility/Debugging.h"

void AMeleeAI::Attack()
{
	if(bCanAttack)
	{
		Super::Attack();
		FHitResult Hit = GetHitDetectionResult(GetCapsuleComponent()->GetComponentLocation());
		if (Hit.bBlockingHit)
		{
			if(Hit.GetActor()->ActorHasTag("Player")) // CHECK IF ENGINE TRACE HAS BEEN ADDED IF THIS DOESN'T FUNCTION
				{
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

void AMeleeAI::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	Blackboard->SetValueAsBool("bCanAttack", bCanAttackPlayer());
}

bool AMeleeAI::bCanAttackPlayer()
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
			if(distance <= AttackRange)
			{
				return true;
			}
		}
	}
	return false;
}
