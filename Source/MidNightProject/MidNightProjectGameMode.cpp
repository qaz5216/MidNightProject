// Copyright Epic Games, Inc. All Rights Reserved.

#include "MidNightProjectGameMode.h"
#include "MidNightProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMidNightProjectGameMode::AMidNightProjectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
