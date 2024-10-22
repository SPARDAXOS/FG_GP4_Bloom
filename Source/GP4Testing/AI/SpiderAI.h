#pragma once

#include "CoreMinimal.h"
#include "MeleeAI.h"
#include "SpiderAI.generated.h"

class UMaterialInstanceDynamic;

UCLASS()
class GP4TESTING_API ASpiderAI : public AMeleeAI {
	GENERATED_BODY()

public:
	ASpiderAI();

public:
	virtual void BeginPlay() override;


public:
	void Dissolve();
	virtual void SetupStartingState() override;

protected:
	virtual void Die() override;
	virtual	void DissolveTimer() override;

private:
	void CreateDynamicMaterials();
	void UpdateDynamicMaterials(const float& value);
	void RandomizeVariation();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Meshes")
	FVector randomScaleMin = FVector(0.8f, 0.8f, 0.8f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Meshes")
	FVector randomScaleMax = FVector(1.2f, 1.2f, 1.2f);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Meshes")
	UStaticMeshComponent* spiderArmsMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Meshes")
	UStaticMeshComponent* spiderEyesMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Meshes")
	UStaticMeshComponent* spiderMaskMesh = nullptr;

protected:
	TObjectPtr<UMaterialInstanceDynamic> bodyDynamicMaterial;
	TObjectPtr<UMaterialInstanceDynamic> spiderArmsDynamicMaterial;
	TObjectPtr<UMaterialInstanceDynamic> spiderEyesDynamicMaterial;
	TObjectPtr<UMaterialInstanceDynamic> spiderMaskDynamicMaterial;

};
