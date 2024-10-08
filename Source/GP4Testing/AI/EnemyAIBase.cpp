// Base class for the enemy AI, to hold all generic functions


#include "EnemyAIBase.h"
#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GP4Testing/WaveManager/GP4_WaveManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyAIBase::AEnemyAIBase()
{
	PrimaryActorTick.bCanEverTick = true;
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
	HealthComponent->OnDeath.AddUniqueDynamic(this, &AEnemyAIBase::Die);
}

// Called when the game starts or when spawned
void AEnemyAIBase::BeginPlay()
{
	Super::BeginPlay();
	Player = GetWorld()->GetFirstPlayerController()->GetCharacter();
	Blackboard = Cast<AAIController>(GetController())->GetBlackboardComponent();
	Blackboard->SetValueAsObject(TEXT("Player"), Player);
}

FHitResult AEnemyAIBase::GetHitDetectionResult() const
{
	FHitResult Hit;
	FVector TraceStart = GetCapsuleComponent()->GetComponentLocation();
	FVector TraceEnd = Player->GetActorLocation();
	FCollisionQueryParams TraceParams;
	
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_GameTraceChannel2, TraceParams);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Emerald, false, 5.f);
	return Hit;
}

// Called every frame
void AEnemyAIBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (GetCharacterMovement()->GetLastUpdateVelocity().Length() > 0)
	{
		bCanPlayAttackAnim = false;
	}
}

// Called to bind functionality to input
void AEnemyAIBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyAIBase::Die()
{
	AGP4_WaveManager* Wave;
	Wave = Cast<AGP4_WaveManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGP4_WaveManager::StaticClass()));
	Wave->OnAIKilled();

	Destroy();
}

void AEnemyAIBase::Attack()
{
	bCanAttack = false;
	bCanPlayAttackAnim = true;
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &AEnemyAIBase::ResetAttack, AttackCooldown, false);
}

void AEnemyAIBase::ResetAttack()
{
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
	bCanPlayAttackAnim = false;
	bCanAttack = true;
}


