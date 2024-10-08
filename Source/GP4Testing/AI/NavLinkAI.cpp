// Fill out your copyright notice in the Description page of Project Settings.


#include "NavLinkAI.h"

#include "EnemyAIBase.h"
#include "NavLinkCustomComponent.h"

ANavLinkAI::ANavLinkAI()
{
	PointLinks.Empty();
	GetSmartLinkComp()->SetEnabled(true);
	bSmartLinkIsRelevant = true;
	OnSmartLinkReached.AddUniqueDynamic(this, &ANavLinkAI::CallJumpFunction);
}

void ANavLinkAI::BeginPlay()
{
	Super::BeginPlay();
}

void ANavLinkAI::CallJumpFunction(AActor* Actor, const FVector& Destination)
{
	AEnemyAIBase* EnemyAI = Cast<AEnemyAIBase>(Actor);
	if (EnemyAI)
	{
		
	}
}
