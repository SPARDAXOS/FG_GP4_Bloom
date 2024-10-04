#include "PickUpGunComponent.h"

UPickUpGunComponent::UPickUpGunComponent()
{
	SphereRadius = 32.f;
}

void UPickUpGunComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UPickUpGunComponent::OnSphereBeginOverlap);
}

void UPickUpGunComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APrimaryPlayer* Character = Cast<APrimaryPlayer>(OtherActor);
	if (Character != nullptr)
	{
		OnPickUp.Broadcast(Character);

		OnComponentBeginOverlap.RemoveAll(this);
	}
}
