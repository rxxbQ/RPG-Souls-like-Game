// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "RPG_Souls_like/AI/AIController/BaseAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "RPG_Souls_like/AI/Blackboard/BlackboardKeys.h"
#include "Kismet/GameplayStatics.h"
#include "RPG_Souls_like/RPG_Souls_likeCharacter.h"

UFindPlayerLocation::UFindPlayerLocation(FObjectInitializer const& ObjectInitializer) 
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UFindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//get player character and AI controller
	ARPG_Souls_likeCharacter* const Player = Cast<ARPG_Souls_likeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	auto const Controller = Cast<ABaseAIController>(OwnerComp.GetAIOwner());

	//get player location to use as an origin
	FVector const PlayerLocation = Player->GetActorLocation();

	if (SearchRandom) {
		FNavLocation Location;

		//get the nagivation system and generate a random location near the player
		UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if (NavSystem->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, Location, nullptr)) {
			Controller->GetBlackboard()->SetValueAsVector(GetSelectedBlackboardKey(), Location.Location);
		}
	}
	else {
		Controller->GetBlackboard()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);
	}

	//finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}
