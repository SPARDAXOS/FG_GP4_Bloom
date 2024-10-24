#pragma once

#include "CoreMinimal.h"
#include "MeleeAI.h"
#include "TyrantAI.generated.h"

class UMaterialInstanceDynamic;

UCLASS()
class GP4TESTING_API ATyrantAI : public AMeleeAI {
	GENERATED_BODY()

public:
	ATyrantAI();

public:
	virtual void BeginPlay() override;


public:
	void Dissolve();


protected:
	virtual void Die() override;
	virtual	void DissolveTimer() override;

private:
	void CreateDynamicMaterials();
	void UpdateDynamicMaterials(const float& value);
	void RandomizeVariation();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Meshes")
	UStaticMeshComponent* spiderArmsMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Meshes")
	UStaticMeshComponent* spiderEyesMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Meshes")
	UStaticMeshComponent* spiderMaskMesh = nullptr;

	UMaterialInstanceDynamic* DynMaterial;

protected:
	TObjectPtr<UMaterialInstanceDynamic> bodyDynamicMaterial;
	TObjectPtr<UMaterialInstanceDynamic> spiderArmsDynamicMaterial;
	TObjectPtr<UMaterialInstanceDynamic> spiderEyesDynamicMaterial;
	TObjectPtr<UMaterialInstanceDynamic> spiderMaskDynamicMaterial;

};
