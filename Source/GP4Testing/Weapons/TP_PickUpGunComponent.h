#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "../Systems/PrimaryPlayer.h"
#include "TP_PickUpGunComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickUpGun, APrimaryPlayer*, PickUpPlayer);

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GP4TESTING_API UTP_PickUpGunComponent : public USphereComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnPickUpGun OnPickUp;

	UTP_PickUpGunComponent();
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};