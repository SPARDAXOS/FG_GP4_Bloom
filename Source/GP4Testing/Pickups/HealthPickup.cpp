

#include "GP4Testing/Pickups/HealthPickup.h"
#include "GP4Testing/Systems/PrimaryPlayer.h"
#include "GP4Testing/Pickups/PickupSpawner.h"

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

		playerRef->GetPlayerHealthSystem().HealthComponent->AddHealth(10);

		if (registeredSpawner)
			registeredSpawner->NotifyPickup(*this);

		Destroy();
		UE_LOG(LogTemp, Warning, TEXT("its health"));
	}
}

