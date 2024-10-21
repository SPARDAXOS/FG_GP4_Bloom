#include "SpiderAI.h"

#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GP4Testing/PlayerSystems/PlayerHealthSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GP4Testing/Systems/PrimaryPlayer.h"
#include "GP4Testing/Systems/WaveManager.h"
#include "GP4Testing/Utility/Debugging.h"
#include "../Utility/Timer.h"
#include "Kismet/GameplayStatics.h"




ASpiderAI::ASpiderAI() {
	PrimaryActorTick.bCanEverTick = true;

	spiderArmsMesh = CreateDefaultSubobject<UStaticMeshComponent>("SpiderArms");
	spiderEyesMesh = CreateDefaultSubobject<UStaticMeshComponent>("SpiderEyes");
	spiderMaskMesh = CreateDefaultSubobject<UStaticMeshComponent>("SpiderMask");

	spiderArmsMesh->SetupAttachment(GetMesh(), "SpiderArms");
	spiderEyesMesh->SetupAttachment(GetMesh(), "SpiderEyes");
	spiderMaskMesh->SetupAttachment(GetMesh(), "SpiderMask");
}


void ASpiderAI::BeginPlay() {
	Super::BeginPlay();

	CreateDynamicMaterials();
	RandomizeVariation();
}
void ASpiderAI::Die() {
	if (WaveManagerSystem)
		WaveManagerSystem->NotifyEnemyDeath(Type);

	if (DeathSound != nullptr)
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());

	bIsDead = true;
	SetActorEnableCollision(false);
	GetCharacterMovement()->GravityScale = 0.0f;
	DissolveTimer();
}
void ASpiderAI::Dissolve()
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

void ASpiderAI::DissolveTimer()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASpiderAI::Dissolve, 0.03f, true);
}


void ASpiderAI::CreateDynamicMaterials() {
	UMaterialInterface* bodyMaterial = GetMesh()->GetMaterial(0);
	if (bodyMaterial) {
		bodyDynamicMaterial = UMaterialInstanceDynamic::Create(bodyMaterial, this);
		GetMesh()->SetMaterial(0, bodyDynamicMaterial);
	}

	UMaterialInterface* armsMaterial = spiderArmsMesh->GetMaterial(0);
	if (armsMaterial) {
		spiderArmsDynamicMaterial = UMaterialInstanceDynamic::Create(armsMaterial, this);
		spiderArmsMesh->SetMaterial(0, spiderArmsDynamicMaterial);
	}

	UMaterialInterface* eyesMaterial = spiderEyesMesh->GetMaterial(0);
	if (eyesMaterial) {
		spiderEyesDynamicMaterial = UMaterialInstanceDynamic::Create(eyesMaterial, this);
		spiderEyesMesh->SetMaterial(0, spiderEyesDynamicMaterial);
	}

	UMaterialInterface* maskMaterial = spiderMaskMesh->GetMaterial(0);
	if (maskMaterial) {
		spiderMaskDynamicMaterial = UMaterialInstanceDynamic::Create(maskMaterial, this);
		spiderMaskMesh->SetMaterial(0, spiderMaskDynamicMaterial);
	}
}
void ASpiderAI::UpdateDynamicMaterials(const float& value) {
	if (bodyDynamicMaterial)
		bodyDynamicMaterial->SetScalarParameterValue("Progress", value);

	if (spiderArmsDynamicMaterial)
		spiderArmsDynamicMaterial->SetScalarParameterValue("Progress", value);

	if (spiderEyesDynamicMaterial)
		spiderEyesDynamicMaterial->SetScalarParameterValue("Progress", value);

	if (spiderMaskDynamicMaterial)
		spiderMaskDynamicMaterial->SetScalarParameterValue("Progress", value);
}
void ASpiderAI::RandomizeVariation() {
	int rand = FMath::RandRange(0, 1);
	if (rand == 0) {
		spiderEyesMesh->SetVisibility(true, true);
		spiderMaskMesh->SetVisibility(false, true);
	}
	else if (rand == 1) {
		spiderEyesMesh->SetVisibility(false, true);
		spiderMaskMesh->SetVisibility(true, true);
	}
}