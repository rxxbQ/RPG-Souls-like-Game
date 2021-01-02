// Fill out your copyright notice in the Description page of Project Settings.


#include "IsPlayerInMeleeRange.h"
#include "RPG_Souls_like/AICharacter.h"
#include "RPG_Souls_like/BossCharacter.h"
#include "RPG_Souls_like/AI/AIController/BaseAIController.h"
#include "Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RPG_Souls_like/AI/Blackboard/BlackboardKeys.h"
#include "RPG_Souls_like/RPG_Souls_likeCharacter.h"
#include "Kismet/GameplayStatics.h"

UIsPlayerInMeleeRange::UIsPlayerInMeleeRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Melee Range");
}

void UIsPlayerInMeleeRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	//get AI controller and AI character
	ABaseAIController* const Controller = Cast<ABaseAIController>(OwnerComp.GetAIOwner());

	if (Controller) {
		if (AAICharacter* const Ch = Cast<AAICharacter>(Controller->GetPawn())) {
			//get the player character
			ARPG_Souls_likeCharacter* const Player = Cast<ARPG_Souls_likeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

			float const Distance = Ch->GetDistanceTo(Player);

			//write true or false depending on whether the player is within MeleeRange
			Controller->GetBlackboard()->SetValueAsBool(GetSelectedBlackboardKey(), Distance <= MeleeRange);
		}
		else if (ABossCharacter* const Boss = Cast<ABossCharacter>(Controller->GetPawn())) {
			ARPG_Souls_likeCharacter* const Player = Cast<ARPG_Souls_likeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			float const Distance = Boss->GetDistanceTo(Player);
			Controller->GetBlackboard()->SetValueAsBool(GetSelectedBlackboardKey(), Distance <= MeleeRange);
		}
		
	}
	
}