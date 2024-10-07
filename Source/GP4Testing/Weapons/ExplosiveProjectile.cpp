#include "ExplosiveProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "GP4Testing/AI/EnemyAIBase.h"
#include "GP4Testing/PlayerSystems/PlayerHealthSystem.h"
#include <GP4Testing/Systems/PrimaryPlayer.h>

AExplosiveProjectile::AExplosiveProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	/*CollisionComp->OnComponentHit.AddDynamic(this, &AExplosiveProjectile::OnHit);*/

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileComp");
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	ExplosionRadiusComp = CreateDefaultSubobject<USphereComponent>("ExplosionRadius");
	ExplosionRadiusComp->SetupAttachment(CollisionComp);

}

void AExplosiveProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AExplosiveProjectile::Explode, FuseTime, false);
}

// For if grenade is sticky
//void AExplosiveProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AExplosiveProjectile::Explode, FuseTime, false);
//}

void AExplosiveProjectile::Explode()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AExplosiveProjectile::DestroyProjectile, 1, false);

	// Check overlapping actors
	TArray<AActor*> OverlappingActors;
	ExplosionRadiusComp->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (AEnemyAIBase* Enemy = Cast<AEnemyAIBase>(Actor))
		{
			Enemy->HealthComponent->TakeDamage(ExplosionDamage);
			UE_LOG(LogTemp, Warning, TEXT("Enemy HP: %f"), Enemy->HealthComponent->CurrentHealth);
		}
		if (APrimaryPlayer* Player = Cast<APrimaryPlayer>(Actor))
		{
			Player->GetPlayerHealthSystem().HealthComponent->TakeDamage(ExplosionDamage);
			UE_LOG(LogTemp, Warning, TEXT("Player HP: %f"), Player->GetPlayerHealthSystem().HealthComponent->CurrentHealth);
		}
	}

}

void AExplosiveProjectile::DestroyProjectile()
{
	Destroy();
}

