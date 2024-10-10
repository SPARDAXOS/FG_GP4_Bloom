#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosiveProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class GP4TESTING_API AExplosiveProjectile : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:	
	AExplosiveProjectile();

	UPROPERTY(VisibleAnywhere, Category = Projectile)
	USphereComponent* ExplosionRadiusComp;

	UPROPERTY(EditAnywhere, Category = Projectile)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void Explode();

	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

private:
	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, Category = "Projectile Settings")
	float FuseTime = 2.f;

	UPROPERTY(EditAnywhere, Category = "Projectile Settings")
	float EnemyDamage = 100;

	UPROPERTY(EditAnywhere, Category = "Projectile Settings")
	float PlayerDamage = 50;
};
