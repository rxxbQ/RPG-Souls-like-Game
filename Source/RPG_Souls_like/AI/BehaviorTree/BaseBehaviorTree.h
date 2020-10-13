// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BaseBehaviorTree.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SOULS_LIKE_API UBaseBehaviorTree : public UBehaviorTree
{
	GENERATED_BODY()

public:
	UBaseBehaviorTree();

protected:

	//blackboard
	class UBaseBlackboardData* BaseBlackboard = NULL;
};
