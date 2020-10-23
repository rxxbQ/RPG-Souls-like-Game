// Fill out your copyright notice in the Description page of Project Settings.


#include "FindRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "RPG_Souls_like/AI/AIController/BaseAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "RPG_Souls_like/AI/Blackboard/BlackboardKeys.h"

UFindRandomLocation::UFindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Random Location");
}

EBTNodeResult::Type UFindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//get AI controller and AI character
	auto const Controller = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
	auto const Character = Controller->GetPawn();

	// obtain AI character location to use as an origin location
	FVector const Origin = Character->GetActorLocation();
	FNavLocation Location;

	//get the navigation system and generate a random location on the NavMesh
	UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, SearchRadius, Location, nullptr)) {
		Controller->GetBlackboard()->SetValueAsVector(GetSelectedBlackboardKey(), Location.Location);
	}

	// finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}
