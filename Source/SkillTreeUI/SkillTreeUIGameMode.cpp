// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkillTreeUIGameMode.h"
#include "SkillTreeUICharacter.h"
#include "UObject/ConstructorHelpers.h"

ASkillTreeUIGameMode::ASkillTreeUIGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
