#include "HealthComponent.h"


void UHealthComponent::BeginPlay() {
	Super::BeginPlay();

}

void UHealthComponent::TakeDamage(float Damage) {
	CurrentHealth -= Damage;
	if (OnDamage.IsBound())
		OnDamage.Broadcast();
	if (CurrentHealth <= 0.0f) {
		CurrentHealth = 0.0f;
		if (OnDeath.IsBound())
			OnDeath.Broadcast();
	}
}

void UHealthComponent::AddHealth(float Amount) {
	if (CurrentHealth < MaxHealth) {
		CurrentHealth += Amount;
		if (CurrentHealth > MaxHealth)
			CurrentHealth = MaxHealth;

		if (OnHeal.IsBound())
			OnHeal.Broadcast();
	}
}


void UHealthComponent::SetupStartingState() noexcept {
	CurrentHealth = MaxHealth;
}