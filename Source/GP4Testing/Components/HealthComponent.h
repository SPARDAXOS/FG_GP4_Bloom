// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP4TESTING_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	void TakeDamage(float Damage);
	void AddHealth(float Amount);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxHealth = 100.f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float CurrentHealth;

	UPROPERTY(BlueprintAssignable)
	FDeathSignature OnDeath;
};
