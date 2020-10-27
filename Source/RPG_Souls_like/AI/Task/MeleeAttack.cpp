// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAttack.h"
//#include "RPG_Souls_like/AI/CombatInterface.h"
#include "RPG_Souls_like/AI/AIController/BaseAIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "Engine/LatentActionManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

UMeleeAttack::UMeleeAttack(FObjectInitializer const& ObjectInitializer) 
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UMeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//get AI controller and AI character
	ABaseAIController* const Controller = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
	
	/*
	if (ICombatInterface* const ICombat = Cast<ICombatInterface>(Ch)) {
		if (MontageHasFinished(Ch)) {
			ICombat->Execute_MeleeAttack(Ch);
		}
	}
	*/
	if (Controller) {
		AAICharacter* const Ch = Cast<AAICharacter>(Controller->GetPawn());

		if (Ch) {
			if (MontageHasFinished(Ch)) {
				Ch->MeleeAttack();
			}
		}

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

bool UMeleeAttack::MontageHasFinished(AAICharacter* const Ch)
{
	return !Ch->GetMesh()->GetAnimInstance()->Montage_IsPlaying(Ch->GetMontage());
}
