// Fill out your copyright notice in the Description page of Project Settings.


#include "IncrementPatrolPathIndex.h"
#include "RPG_Souls_like/AI/AIController/BaseAIController.h"
#include "RPG_Souls_like/AI/Blackboard/BlackboardKeys.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RPG_Souls_like/AICharacter.h"
#include <cmath>

UIncrementPatrolPathIndex::UIncrementPatrolPathIndex(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Increment Path Index");
}

EBTNodeResult::Type UIncrementPatrolPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//get the AI controller
	ABaseAIController* const Controller = Cast<ABaseAIController>(OwnerComp.GetAIOwner());

	if (Controller) {
		AAICharacter* const Ch = Cast<AAICharacter>(Controller->GetPawn());

		if (Ch) {
			if (Ch->NeedToPatrol) {
				int const NumOfPoints = Ch->GetPatrolPath()->Num();
				int const MinIndex = 0;
				int const MaxIndex = NumOfPoints - 1;

				//get and set the blackboard index key
				int Index = Controller->GetBlackboard()->GetValueAsInt(GetSelectedBlackboardKey());
				if (Bidirectional) {
					if (Index >= MaxIndex && Direction == EDirectionType::Forward) {
						Direction = EDirectionType::Reverse;
					}
					else if (Index == MinIndex && Direction == EDirectionType::Reverse) {
						Direction = EDirectionType::Forward;
					}
				}

				Controller->GetBlackboard()->SetValueAsInt(GetSelectedBlackboardKey(), (Direction == EDirectionType::Forward ? std::abs(++Index) : std::abs(--Index)) % NumOfPoints);

			}
			//finish with success
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;

		}
	}
	return EBTNodeResult::Failed;
}