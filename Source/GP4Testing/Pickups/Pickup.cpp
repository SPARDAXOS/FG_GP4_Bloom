// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickUpRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PickUpRoot"));
	RootComponent = PickUpRoot;

	PickUpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickUpMesh"));
	PickUpMesh->SetupAttachment(PickUpRoot);
	
	PickUpBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PickUpBox"));
	PickUpBox->SetupAttachment(RootComponent);
	PickUpBox->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	PickUpBox->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnPlayerInteraction);

	vfx = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFX"));
	vfx->SetupAttachment(RootComponent);
	vfx->bAutoActivate = true;

	RotationRate = FRotator(0.0f, 90.0f, 0.0F);
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void APickup::OnPlayerInteraction(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* otherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResults)
{
	OnPickup(OverlappedComp, OtherActor, otherComp, OtherBodyIndex, bFromSweep, SweepResults);
	
}
void APickup::OnPickup(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* otherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResults) 
{

}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalRotation(RotationRate * DeltaTime);

	float hight = 0.2 * FMath::Sin(GetWorld()->GetTimeSeconds());
	FVector pickupPosition = PickUpRoot->GetRelativeLocation();
	FVector Movement = FVector(pickupPosition.X, pickupPosition.Y, hight + pickupPosition.Z);
	PickUpRoot->SetRelativeLocation(Movement);

}

