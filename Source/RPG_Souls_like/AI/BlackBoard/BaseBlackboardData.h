// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardData.h"
#include "BaseBlackboardData.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SOULS_LIKE_API UBaseBlackboardData : public UBlackboardData
{
	GENERATED_BODY()

public:
	UBaseBlackboardData();

protected:

	//is moving
	class UBlackboardKeyType_Bool* IsMove;

	//is idling
	class UBlackboardKeyType_Bool* IsIdle;

	class UBlackboardKeyType_Vector* EnemyLocation;
};
