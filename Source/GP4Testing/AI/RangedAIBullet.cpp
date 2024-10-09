// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedAIBullet.h"

#include "Chaos/Interface/SQTypes.h"
#include "GP4Testing/PlayerSystems/PlayerHealthSystem.h"
#include "GP4Testing/Systems/PrimaryPlayer.h"
#include "GP4Testing/Utility/Debugging.h"

// Sets default values
ARangedAIBullet::ARangedAIBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ARangedAIBullet::HandleBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ARangedAIBullet::HandeEndOverlap);
	RootComponent = Sphere;
}

// Called when the game starts or when spawned
void ARangedAIBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARangedAIBullet::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Debugging::PrintString("Hit an object");
	APrimaryPlayer* Player = Cast<APrimaryPlayer>(OtherActor);
	if (Player)
	{
		Debugging::PrintString("Hit player");
		Player->GetPlayerHealthSystem().HealthComponent->TakeDamage(Damage);
	}
	
	Destroy();
}

void ARangedAIBullet::HandeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

// Called every frame
void ARangedAIBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalOffset(FVector::ForwardVector * Speed * DeltaTime);
}

void ARangedAIBullet::SetDamage(float NewDamage)
{
	Damage = NewDamage;
}

