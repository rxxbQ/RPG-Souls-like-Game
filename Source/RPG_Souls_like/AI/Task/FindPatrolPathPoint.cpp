// Fill out your copyright notice in the Description page of Project 


#include "FindPatrolPathPoint.h"
#include "RPG_Souls_like/AI/AIController/BaseAIController.h"
#include "RPG_Souls_like/AI/Blackboard/BlackboardKeys.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RPG_Souls_like/AICharacter.h"

UFindPatrolPathPoint::UFindPatrolPathPoint(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Patrol Path Point");
}

EBTNodeResult::Type UFindPatrolPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemeory)
{
	//get the AI controller for the patrolling AI
	ABaseAIController* const Controller = Cast<ABaseAIController>(OwnerComp.GetAIOwner());

	if (Controller) {
		//get the current patrol path index from the blackboard
		int const Index = Controller->GetBlackboard()->GetValueAsInt(GetSelectedBlackboardKey());

		//use the index to get the current patrol path from the AI's reference to the patrol path
		AAICharacter* const Ch = Cast<AAICharacter>(Controller->GetPawn());

		if (Ch) {
			FVector const Point = Ch->GetPatrolPath()->GetPatrolPoint(Index);

			//transform this point to a global position using its parent
			FVector const GlobalPoint = Ch->GetPatrolPath()->GetActorTransform().TransformPosition(Point);

			// write the current global path point to the blackboard
			Controller->GetBlackboard()->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);

			//finish with success
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
		
	}
	return EBTNodeResult::Failed;
}
