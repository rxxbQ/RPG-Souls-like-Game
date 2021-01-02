// Fill out your copyright notice in the Description page of Project Settings.


#include "ChangeSpeed.h"
#include "RPG_Souls_like/AICharacter.h"
#include "RPG_Souls_like/AI/AIController/BaseAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

UChangeSpeed::UChangeSpeed() 
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}

void UChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	auto const Controller = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
	if (Controller) {
		if (AAICharacter* const Ch = Cast<AAICharacter>(Controller->GetPawn())) {
			Ch->GetCharacterMovement()->MaxWalkSpeed = Speed;
		}
	}
	
}

FString UChangeSpeed::GetStaticServiceDescription() const
{
	return FString("Change AI Speed");
}
