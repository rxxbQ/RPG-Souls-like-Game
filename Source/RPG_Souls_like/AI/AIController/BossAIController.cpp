// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAIController.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AISenseConfig_Sight.h"

ABossAIController::ABossAIController(FObjectInitializer const& ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeObject(TEXT("BehaviorTree'/Game/AI/Boss/BossBT.BossBT'"));

	if (BehaviorTreeObject.Succeeded()) {
		BehaviorTree = BehaviorTreeObject.Object;
	}

	SetupPerceptionSystem();
}

void ABossAIController::SetupPerceptionSystem()
{
	ABaseAIController::SetupPerceptionSystem();

	SightConfig->SightRadius = 2500.0f;
	//SightConfig->PeripheralVisionAngleDegrees = 360.0f;
}
