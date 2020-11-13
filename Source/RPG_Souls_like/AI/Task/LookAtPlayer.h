// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "LookAtPlayer.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SOULS_LIKE_API ULookAtPlayer : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	ULookAtPlayer();

	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
