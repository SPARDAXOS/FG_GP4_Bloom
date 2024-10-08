// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavLinkProxy.h"
#include "NavLinkAI.generated.h"

/**
 * 
 */
UCLASS()
class GP4TESTING_API ANavLinkAI : public ANavLinkProxy
{
	GENERATED_BODY()
	ANavLinkAI();
	virtual void BeginPlay() override;
	void CallJumpFunction(AActor* Actor, const FVector& Destination);
};
