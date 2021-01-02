// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPG_Souls_likeGameMode.h"
#include "PaladinCharacter.h"
#include "BossCharacter.h"
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
	
	/*
	static ConstructorHelpers::FClassFinder<AAICharacter> AIClass(TEXT("Blueprint'/Game/MyAICharacter.MyAICharacter_C'"));
	if (AIClass.Succeeded()) {
		FAICharacterClass = AIClass.Class;
	}

	static ConstructorHelpers::FClassFinder<APatrolPath> PatrolClass(TEXT("Blueprint'/Game/AI/BP/MyPatrolPath.MyPatrolPath_C'"));
	if (PatrolClass.Succeeded()) {
		FPatrolPathClass = PatrolClass.Class;
	}
	*/
}
/*
void ARPG_Souls_likeGameMode::BeginPlay()
{
	Super::BeginPlay();
	if (FAICharacterClass && FPatrolPathClass) {
		FVector const AILocation{ -1011.29126f, -703.557251f, 226.001602f };
		FVector const PatrolPathLocation{ -605.f, -1110.f, 130.f };
		FActorSpawnParameters const Params;

		if (auto const Path = GetWorld()->SpawnActor<APatrolPath>(FPatrolPathClass, PatrolPathLocation, FRotator::ZeroRotator, Params)) {
			Path->AddPatrolPoint(FVector{0.f, 0.f, 0.f});
			Path->AddPatrolPoint(FVector{1550.f, -30.f, 0.f});
			Path->AddPatrolPoint(FVector{1580.f, 2290.f, 0.f});
			Path->AddPatrolPoint(FVector{90.f, 2350.f, 0.f});
			Path->AddPatrolPoint(FVector{0.f, 1150.f, 0.f});

			if (auto const AICharacter = GetWorld()->SpawnActor<AAICharacter>(FAICharacterClass, AILocation, FRotator::ZeroRotator, Params)) {
				AICharacter->SetPatrolPath(Path);
			}
		}
	}
}
*/