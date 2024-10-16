// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAI.h"

#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GP4Testing/PlayerSystems/PlayerHealthSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GP4Testing/Systems/PrimaryPlayer.h"
#include "GP4Testing/Utility/Debugging.h"
#include "../Utility/Timer.h"

void AMeleeAI::Die()
{
	Super::Die();
	bIsDead = true;
	SetActorEnableCollision(false);
	GetCharacterMovement()->GravityScale = 0.0f;
	DynMaterial = UMaterialInstanceDynamic::Create(DissolveMat, this);
	DissolveTimer();
}

void AMeleeAI::SetEnemyState(bool state)
{
	bIsDead = !state;
	Super::SetEnemyState(state);
}

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
	Blackboard->SetValueAsBool("bIsDead", bIsDead);
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

void AMeleeAI::Dissolve()
{
	DissolveValue += 0.01f;

	if (DissolveValue >= 1)
	{
		DissolveValue = 0.0f;
		GetMesh()->SetMaterial(0, GetMesh()->GetMaterial(0));

		if (DynMaterial != nullptr)
		{
			DynMaterial->SetScalarParameterValue("Progress", DissolveValue);
			GetMesh()->SetMaterial(0, DynMaterial);
		}

		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		SetEnemyState(false);
	}

	if (DynMaterial != nullptr)
	{
		DynMaterial->SetScalarParameterValue("Progress", DissolveValue);
		GetMesh()->SetMaterial(0, DynMaterial);
	}
}

void AMeleeAI::DissolveTimer()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMeleeAI::Dissolve, 0.03f, true);
}
