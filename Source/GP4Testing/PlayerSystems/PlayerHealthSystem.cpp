

#include "PlayerHealthSystem.h"

#include "GP4Testing/Systems/PrimaryPlayer.h"

APlayerHealthSystem::APlayerHealthSystem()
{
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
	HealthComponent->OnDeath.AddUniqueDynamic(this, &APlayerHealthSystem::KillPlayer);
}

void APlayerHealthSystem::SetupStartingState() noexcept {

}

void APlayerHealthSystem::KillPlayer()
{
	primaryPlayerRef->SetPlayerState(false);
	//HealthComponent->CurrentHealth = HealthComponent->MaxHealth;
}
