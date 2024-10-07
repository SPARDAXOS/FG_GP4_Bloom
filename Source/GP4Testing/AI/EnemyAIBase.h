// Base class for the enemy AI, to hold all generic functions

#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GP4Testing/Components/HealthComponent.h"
#include "EnemyAIBase.generated.h"

UCLASS()
class GP4TESTING_API AEnemyAIBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyAIBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FHitResult GetHitDetectionResult() const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Die();

	UFUNCTION(BlueprintCallable)
	virtual void Attack();

	UFUNCTION(BlueprintCallable)
	virtual void ResetAttack();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanAttack = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanPlayAttackAnim = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Damage = 30;

	UPROPERTY(EditDefaultsOnly)
	float AttackRange = 140;

	UPROPERTY(EditDefaultsOnly)
	float AttackCooldown = 1;

	UPROPERTY(VisibleAnywhere)
	ACharacter* Player = nullptr;

	UFUNCTION(BlueprintCallable)
	void SetPlayerRef(ACharacter* PlayerCharacter);

	UBlackboardComponent* Blackboard = nullptr;

	FTimerHandle AttackTimerHandle;

	UHealthComponent* HealthComponent = nullptr;

	bool PlayerHasBeenSet = false;

};
