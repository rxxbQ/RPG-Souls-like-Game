// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG_Souls_like/AI/AIController/BaseAIController.h"
#include "BossAIController.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SOULS_LIKE_API ABossAIController : public ABaseAIController
{
	GENERATED_BODY()
	
public:
	ABossAIController(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get());

	virtual void SetupPerceptionSystem();
};
