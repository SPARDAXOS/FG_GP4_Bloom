// Fill out your copyright notice in the Description page of Project Settings.


#include "GP4Testing/VFXEntities/ImpactVFX.h"
#include "NiagaraComponent.h"
#include "GP4Testing/Weapons/GunComponent.h"
// Sets default values
AImpactVFX::AImpactVFX()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//AGunComponent* gunCompnent = Cast<AGunComponent>(gunCompnent);

	/*VFX2->SetWorldLocation(gunCompnent->Fire().Hit.Location);
	VFX2->SetWorldRotation(Hit.Normal.ToOrientationQuat());
	VFX2->Activate(true);*/

}

// Called when the game starts or when spawned
void AImpactVFX::BeginPlay()
{
	Super::BeginPlay();

	
	
}

// Called every frame
void AImpactVFX::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

