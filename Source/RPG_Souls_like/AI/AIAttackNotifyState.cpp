// Fill out your copyright notice in the Description page of Project Settings.


#include "AIAttackNotifyState.h"
#include "Engine/Engine.h"
#include "Components/SkeletalMeshComponent.h"
#include "RPG_Souls_like/AICharacter.h"

void UAIAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner()) {
		if (AAICharacter* const Character = Cast<AAICharacter>(MeshComp->GetOwner())) {
			Character->AttackStart();
		}
	}
}

void UAIAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner()) {
		if (AAICharacter* const Character = Cast<AAICharacter>(MeshComp->GetOwner())) {
			Character->AttackEnd();
		}
	}

}
