// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PuzzlePlatformGameMode.h"
#include "PuzzlePlatformCharacter.h"
#include "UObject/ConstructorHelpers.h"

APuzzlePlatformGameMode::APuzzlePlatformGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ParagonKwang/Characters/Heroes/Kwang/KwangPlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/PuzzlePlatform/BP_PuzzlePlayerController"));
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ParagonKwang/Characters/Heroes/Kwang/KwangPlayerCharacter"));
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
		
	}
}