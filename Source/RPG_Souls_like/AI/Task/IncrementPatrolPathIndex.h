// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "IncrementPatrolPathIndex.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SOULS_LIKE_API UIncrementPatrolPathIndex : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UIncrementPatrolPathIndex(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	enum class EDirectionType {
		Forward, 
		Reverse
	};

	EDirectionType Direction = EDirectionType::Forward;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		bool Bidirectional = true;
};
