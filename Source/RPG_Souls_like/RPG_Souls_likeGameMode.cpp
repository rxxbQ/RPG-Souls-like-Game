// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPG_Souls_likeGameMode.h"
#include "PaladinCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "CharacterPlayerController.h"

ARPG_Souls_likeGameMode::ARPG_Souls_likeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APaladinCharacter> PlayerPawnBPClass(TEXT("Blueprint'/Game/MyPaladinCharacter.MyPaladinCharacter_C'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		//DefaultPawnClass = APaladinCharacter::StaticClass();
		PlayerControllerClass = ACharacterPlayerController::StaticClass();
	}
	
}
