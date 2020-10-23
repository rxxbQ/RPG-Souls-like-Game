// Fill out your copyright notice in the Description page of Project Settings.


#include "CastSpellAnimNotifyState.h"
#include "RPG_Souls_likeCharacter.h"
#include "GameFramework/Controller.h"

void UCastSpellAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner()) {
		ARPG_Souls_likeCharacter* Player = Cast<ARPG_Souls_likeCharacter>(MeshComp->GetOwner());

		if (Player) {
			Player->CastSpellStart();
		}
	}
}

void UCastSpellAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner()) {
		ARPG_Souls_likeCharacter* Player = Cast<ARPG_Souls_likeCharacter>(MeshComp->GetOwner());

		if (Player) {
			Player->CastSpellEnd();
		}
	}
}
