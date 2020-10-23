// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "FindPatrolPathPoint.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SOULS_LIKE_API UFindPatrolPathPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
    UFindPatrolPathPoint(FObjectInitializer const& ObjectInitializer);

    EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemeory) override;

private:
    UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = "true"))
        struct FBlackboardKeySelector PatrolPathVectorKey;
};
