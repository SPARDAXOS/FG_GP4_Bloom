

#include "GP4Testing/Pickups/HealthPickup.h"
#include "GP4Testing/Systems/PrimaryPlayer.h"

AHealthPickup::AHealthPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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
		Destroy();
		UE_LOG(LogTemp, Warning, TEXT("its health"));
	}
}

