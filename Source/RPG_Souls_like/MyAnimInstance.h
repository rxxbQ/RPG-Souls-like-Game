// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SOULS_LIKE_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds);
	
	UPROPERTY(BlueprintReadOnly)
		uint8 MoveSpeed;

protected:
	//detect character is moving
	UPROPERTY(BlueprintReadOnly)
	float IsMove;

	UPROPERTY(BlueprintReadOnly)
	float Direction;

	UPROPERTY(BlueprintReadOnly)
		bool IsInAir;
};
