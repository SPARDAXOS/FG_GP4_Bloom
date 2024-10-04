// Copyright Epic Games, Inc. All Rights Reserved.

#include "GP4TestingGameMode.h"
#include "GP4TestingCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGP4TestingGameMode::AGP4TestingGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
