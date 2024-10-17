

#include "GP4Testing/Pickups/HealthPickup.h"
#include "GP4Testing/Systems/PrimaryPlayer.h"
#include "GP4Testing/Pickups/PickupSpawner.h"
#include "Kismet/GameplayStatics.h"

AHealthPickup::AHealthPickup()
{
	PrimaryActorTick.bCanEverTick = true;
	pickupType = PickupType::HEALTH;

	
}
void AHealthPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AHealthPickup::OnPickup(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* otherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResults) 
{
	if (OtherActor->ActorHasTag("Player"))
	{


		APrimaryPlayer* playerRef = Cast<APrimaryPlayer>(OtherActor);
		if (!playerRef)
			return;

		if (playerRef->GetPlayerHealthSystem().HealthComponent->CurrentHealth < playerRef->GetPlayerHealthSystem().HealthComponent->MaxHealth)
		{
			playerRef->GetPlayerHealthSystem().HealthComponent->AddHealth(10);

			if (registeredSpawner)
				registeredSpawner->NotifyPickup(*this);

			Destroy();

			if (PickupSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, PickupSound, playerRef->GetActorLocation());
			}

			UE_LOG(LogTemp, Warning, TEXT("its health"));
		}	
	}
}

