// Base class for the enemy AI, to hold all generic functions


#include "EnemyAIBase.h"
#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GP4Testing/Utility/Debugging.h"
//#include "GP4Testing/WaveManager/GP4_WaveManager.h"
#include "GP4Testing/Weapons/GunComponent.h"
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

FHitResult AEnemyAIBase::GetHitDetectionResult(FVector Location) const
{
	FHitResult Hit;
	FVector TraceStart = Location;
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
	Blackboard->SetValueAsBool("bHasRecentlyLanded", bHasRecentlyLanded);
	if (GetCharacterMovement()->GetLastUpdateVelocity().Length() > 0)
	{
		bCanPlayAttackAnim = false;
	}

	if(GetCharacterMovement()->IsMovingOnGround())
	{
		bJumped = false;
	}
}

// Called to bind functionality to input
void AEnemyAIBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyAIBase::Die()
{
	//AWaveManager* Wave;
	//Wave = Cast<AWaveManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWaveManager::StaticClass()));
	//if (Wave)
	//{
	//	Wave->OnAIKilled();
	//}
	SetEnemyState(false);
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

void AEnemyAIBase::NavLinkJump(const FVector& Destination)
{
	FVector OutLaunch;
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(),  OutLaunch, GetActorLocation(), Destination);
	OutLaunch.Z = OutLaunch.Z * JumpForce;
	LaunchCharacter(OutLaunch, true, true);
	bJumped = true;
}

void AEnemyAIBase::SetEnemyState(bool state)
{
	SetActorTickEnabled(state);
	SetActorEnableCollision(state);
	SetActorHiddenInGame(!state);

	GetCapsuleComponent()->SetEnableGravity(state);
	GetCharacterMovement()->SetActive(state);
	if (state)
		GetCharacterMovement()->GravityScale =	1.0f;	//defaultGravityScale;
	else
		GetCharacterMovement()->GravityScale = 0.0f;

	Active = state;
}

void AEnemyAIBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	bHasRecentlyLanded = true;
	GetWorld()->GetTimerManager().SetTimer(LandingTimerHandle, this, &AEnemyAIBase::ResetLandingState, LandingMovementCooldown, false);
}

void AEnemyAIBase::ResetLandingState()
{
	bHasRecentlyLanded = false;
	GetWorld()->GetTimerManager().ClearTimer(LandingTimerHandle);
}




