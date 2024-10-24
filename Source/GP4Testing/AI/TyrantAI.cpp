#include "TyrantAI.h"

#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GP4Testing/Systems/PrimaryPlayer.h"
#include "GP4Testing/PlayerSystems/PlayerHealthSystem.h"
#include "GP4Testing/Systems/WaveManager.h"
#include "GP4Testing/Utility/Debugging.h"


#include "../Utility/Timer.h"
#include "Kismet/GameplayStatics.h"




ATyrantAI::ATyrantAI() {
	PrimaryActorTick.bCanEverTick = true;

	helmetMesh = CreateDefaultSubobject<UStaticMeshComponent>("Helmet");

	helmetMesh->SetupAttachment(GetMesh(), "Helmet");
}


void ATyrantAI::BeginPlay() {
	Super::BeginPlay();

	CreateDynamicMaterials();
	RandomizeVariation();
}
void ATyrantAI::Die() {
	if (WaveManagerSystem)
		WaveManagerSystem->NotifyEnemyDeath(Type);

	if (DeathSound != nullptr)
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());

	bIsDead = true;
	SetActorEnableCollision(false);
	GetCharacterMovement()->GravityScale = 0.0f;
	DynMaterial = UMaterialInstanceDynamic::Create(DissolveMat, this);
	DissolveTimer();
}
void ATyrantAI::Dissolve()
{
	DissolveValue += 0.01f; //???

	if (DissolveValue >= 1) //???
	{
		DissolveValue = 0.0f; //???

		UpdateDynamicMaterials(DissolveValue);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		SetEnemyState(false);
	}
	else
		UpdateDynamicMaterials(DissolveValue);
}

void ATyrantAI::DissolveTimer() {
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATyrantAI::Dissolve, 0.03f, true);
}


void ATyrantAI::CreateDynamicMaterials() {
	UMaterialInterface* bodyMaterial = GetMesh()->GetMaterial(0);
	if (bodyMaterial) {
		bodyDynamicMaterial = UMaterialInstanceDynamic::Create(bodyMaterial, this);
		GetMesh()->SetMaterial(0, bodyDynamicMaterial);
	}

	UMaterialInterface* helmetMaterial = helmetMesh->GetMaterial(0);
	if (helmetMaterial) {
		helmetDynamicMaterial = UMaterialInstanceDynamic::Create(helmetMaterial, this);
		helmetMesh->SetMaterial(0, helmetDynamicMaterial);
	}
}
void ATyrantAI::UpdateDynamicMaterials(const float& value) {
	if (bodyDynamicMaterial)
		bodyDynamicMaterial->SetScalarParameterValue("Progress", value);

	if (helmetDynamicMaterial)
		helmetDynamicMaterial->SetScalarParameterValue("Progress", value);
}
void ATyrantAI::RandomizeVariation() {
	int rand = FMath::RandRange(0, 1);
	if (rand == 0) {
		helmetMesh->SetVisibility(true, true);
	}
	else if (rand == 1) {
		helmetMesh->SetVisibility(false, true);
	}
}