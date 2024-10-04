// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "RangedAIBullet.generated.h"

UCLASS()
class GP4TESTING_API ARangedAIBullet : public AActor //Add collision to cast to player class and then apply damage through it
{
	GENERATED_BODY()
	
public:	
	ARangedAIBullet();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void SetDamage(float NewDamage);

private:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float Speed = 1000.f;

	float Damage;
};
