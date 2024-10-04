#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "../Systems/PrimaryPlayer.h"
#include "PickUpGunComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickUpGun, APrimaryPlayer*, PickUpPlayer);

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GP4TESTING_API UPickUpGunComponent : public USphereComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnPickUpGun OnPickUp;

	UPickUpGunComponent();
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};