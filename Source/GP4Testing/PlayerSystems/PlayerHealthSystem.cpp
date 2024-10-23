

#include "PlayerHealthSystem.h"

#include "WeaponManagementSystem.h"
#include "GP4Testing/Systems/PrimaryGameMode.h"

#include "GP4Testing/Systems/PrimaryPlayer.h"

APlayerHealthSystem::APlayerHealthSystem()
{
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
	HealthComponent->OnDeath.AddUniqueDynamic(this, &APlayerHealthSystem::KillPlayer);
}

void APlayerHealthSystem::SetupStartingState() noexcept {
	HealthComponent->SetupStartingState();
}

void APlayerHealthSystem::KillPlayer()
{
	if (primaryPlayerRef->GetWeaponManagementSystem().GetAcquiredWeapons().Num() > 0)
		primaryPlayerRef->GetWeaponManagementSystem().GetCurrentWeapon()->StopFire();
	primaryPlayerRef->SetPlayerState(false);
	primaryPlayerRef->GetPrimaryGameMode()->CompleteGame(GameResults::LOSE);

}
