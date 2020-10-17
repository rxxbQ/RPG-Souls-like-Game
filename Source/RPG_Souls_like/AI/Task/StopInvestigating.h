// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "StopInvestigating.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SOULS_LIKE_API UStopInvestigating : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UStopInvestigating(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
