// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "RPG_Souls_likeCharacter.h"

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds) 
{
	ARPG_Souls_likeCharacter* Ch = Cast<ARPG_Souls_likeCharacter>(TryGetPawnOwner());
	
	if (Ch) {
		FVector Length = Ch->GetVelocity();
		IsMove = Length.Size();

		FRotator Rotator = Ch->GetActorRotation();
		
		Direction = CalculateDirection(Length, Rotator);
	}
}