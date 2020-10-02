// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "TaskItemActor.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SOULS_LIKE_API ATaskItemActor : public AItemActor
{
	GENERATED_BODY()

public:
	ATaskItemActor();
	
	virtual bool UseItem();
};
