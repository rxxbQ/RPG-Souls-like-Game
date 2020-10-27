// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AICharacter.h"
#include "GameFramework/GameModeBase.h"
#include "RPG_Souls_likeGameMode.generated.h"

UCLASS(minimalapi)
class ARPG_Souls_likeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARPG_Souls_likeGameMode();

	//void BeginPlay() override;
/*
private:
	TSubclassOf<AAICharacter> FAICharacterClass;

	TSubclassOf<APatrolPath> FPatrolPathClass;
*/
};



