// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionComponent.h"
#include "Engine/GameEngine.h"

AEnemyAIController::AEnemyAIController(FObjectInitializer const& ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeObject(TEXT("BehaviorTree'/Game/AI/Behavior/BaseBT.BaseBT'"));

	if (BehaviorTreeObject.Succeeded()) {
		BehaviorTree = BehaviorTreeObject.Object;
	}

	TeamId = FGenericTeamId(1);

	SetupPerceptionSystem();
}



void AEnemyAIController::SetupPerceptionSystem()
{
	ABaseAIController::SetupPerceptionSystem();

	//create and initialize hearing config object
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));

	if (HearingConfig) {
		HearingConfig->HearingRange = 1000.0f;
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

		//add hearing configuration component to perception component
		
		GetPerceptionComponent()->ConfigureSense(*HearingConfig);
	}
}
