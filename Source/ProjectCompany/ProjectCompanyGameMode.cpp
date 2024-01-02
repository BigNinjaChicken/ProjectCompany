// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectCompanyGameMode.h"
#include "ProjectCompanyCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectCompanyGameMode::AProjectCompanyGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
