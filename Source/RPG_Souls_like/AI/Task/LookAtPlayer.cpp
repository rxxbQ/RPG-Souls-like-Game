// Fill out your copyright notice in the Description page of Project Settings.


#include "LookAtPlayer.h"
#include "RPG_Souls_like/AICharacter.h"
#include "RPG_Souls_like/AI/AIController/BaseAIController.h"
#include "Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RPG_Souls_like/AI/Blackboard/BlackboardKeys.h"
#include "RPG_Souls_like/RPG_Souls_likeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ULookAtPlayer::ULookAtPlayer()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Look At Player");
}

void ULookAtPlayer::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	//get AI controller
	ABaseAIController* const Controller = Cast<ABaseAIController>(OwnerComp.GetAIOwner());

	if (Controller) {
		//get AI character
		AAICharacter* const Ch = Cast<AAICharacter>(Controller->GetPawn());

		if (Ch) {
			// get player character
			ARPG_Souls_likeCharacter* const Player = Cast<ARPG_Souls_likeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

			FRotator StrafeRotator = UKismetMathLibrary::FindLookAtRotation(Ch->GetActorLocation(), Player->GetActorLocation());
			Ch->SetActorRotation(StrafeRotator);
		}
	}
}
