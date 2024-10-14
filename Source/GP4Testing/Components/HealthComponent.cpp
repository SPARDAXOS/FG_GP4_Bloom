// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

void UHealthComponent::TakeDamage(float Damage)
{
	CurrentHealth -= Damage;
	if(OnDamage.IsBound())
		OnDamage.Broadcast();
	if(CurrentHealth <= 0.f)
	{
		CurrentHealth = 0.f;
		if (OnDeath.IsBound())
			OnDeath.Broadcast();
	}
}

void UHealthComponent::AddHealth(float Amount)
{
	if(CurrentHealth != MaxHealth)
	{
		CurrentHealth += Amount;
		if (OnHeal.IsBound())
			OnHeal.Broadcast();
	}
}
