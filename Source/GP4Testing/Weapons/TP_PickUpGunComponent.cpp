#include "TP_PickUpGunComponent.h"

UTP_PickUpGunComponent::UTP_PickUpGunComponent()
{
	SphereRadius = 32.f;
}

void UTP_PickUpGunComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UTP_PickUpGunComponent::OnSphereBeginOverlap);
}

void UTP_PickUpGunComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APrimaryPlayer* Character = Cast<APrimaryPlayer>(OtherActor);
	if (Character != nullptr)
	{
		OnPickUp.Broadcast(Character);

		OnComponentBeginOverlap.RemoveAll(this);
	}
}
