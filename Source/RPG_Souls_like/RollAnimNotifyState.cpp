// Fill out your copyright notice in the Description page of Project Settings.


#include "RollAnimNotifyState.h"
#include "Engine/Engine.h"
#include "Components/SkeletalMeshComponent.h"
#include "RPG_Souls_likeCharacter.h"

void URollAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner()) {
		if (ARPG_Souls_likeCharacter* const Character = Cast<ARPG_Souls_likeCharacter>(MeshComp->GetOwner())) {
			Character->RollStart();
		}
	}
}
void URollAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner()) {
		if (ARPG_Souls_likeCharacter* const Character = Cast<ARPG_Souls_likeCharacter>(MeshComp->GetOwner())) {
			Character->RollEnd();
		}
	}
}
