// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackAnimNotifyState.h"
#include "RPG_Souls_likeCharacter.h"
#include "GameFramework/Controller.h"

void UAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) 
{
	if (MeshComp && MeshComp->GetOwner()) {
		ARPG_Souls_likeCharacter* Player = Cast<ARPG_Souls_likeCharacter>(MeshComp->GetOwner());
		if (Player) {
			Player->AttackStart();
		}
	}
}

void UAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) 
{
	if (MeshComp && MeshComp->GetOwner()) {
		ARPG_Souls_likeCharacter* Player = Cast<ARPG_Souls_likeCharacter>(MeshComp->GetOwner());
		if (Player) {
			Player->AttackEnd();
		}
	}
}
