// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasePlayer.h"
#include "RPG_Souls_like/AI/AIController/BaseAIController.h"
#include "RPG_Souls_like/AI/Blackboard/BlackboardKeys.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UChasePlayer::UChasePlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//get TargetLocation from blackboard via AI controller
	ABaseAIController* const Controller = Cast<ABaseAIController>(OwnerComp.GetAIOwner());

	if (Controller) {
		FVector const PlayerLocation = Controller->GetBlackboard()->GetValueAsVector(GetSelectedBlackboardKey());

		//move to the player's location
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, PlayerLocation);

		//finish with succcess
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
	
}
