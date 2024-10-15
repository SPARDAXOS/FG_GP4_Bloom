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
	DissolveTimer();
	SetEnemyState(false);
}

void AMeleeAI::SetEnemyState(bool state)
{
	SetActorTickEnabled(state);
	SetActorEnableCollision(state);
	ResetDissolve();

	GetCapsuleComponent()->SetEnableGravity(state);
	GetCharacterMovement()->SetActive(state);
	if (state)
		GetCharacterMovement()->GravityScale = 1.0f;	//defaultGravityScale;
	else
		GetCharacterMovement()->GravityScale = 0.0f;

	Active = state;
	Blackboard->SetValueAsBool("Active", Active);
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

	if (DissolveValue == 1)
	{
		SetActorHiddenInGame(true);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
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
	UE_LOG(LogTemp, Warning, TEXT("IS THIS WORKING???"))
	DissolveValue += 0.01f;
	UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(DissolveMat, this);
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

void AMeleeAI::ResetDissolve()
{
	DissolveValue = 0;
	UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(DissolveMat, this);
	if (DynMaterial != nullptr)
	{
		DynMaterial->SetScalarParameterValue("Progress", DissolveValue);
		GetMesh()->SetMaterial(0, DynMaterial);
	}
}
