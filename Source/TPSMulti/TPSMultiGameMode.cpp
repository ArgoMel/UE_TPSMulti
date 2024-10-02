// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPSMultiGameMode.h"
#include "TPSMultiCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATPSMultiGameMode::ATPSMultiGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
