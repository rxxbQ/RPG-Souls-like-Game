// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG_Souls_like/AI/AIController/BaseAIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SOULS_LIKE_API AEnemyAIController : public ABaseAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get());

private:
	virtual void SetupPerceptionSystem();

	//hearing sense
	class UAISenseConfig_Hearing* HearingConfig;

};
