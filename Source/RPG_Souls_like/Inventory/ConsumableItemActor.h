// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "ConsumableItemActor.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SOULS_LIKE_API AConsumableItemActor : public AItemActor
{
	GENERATED_BODY()

public:
    AConsumableItemActor();

    virtual bool UseItem();

    void SetupHealth();

    ACharacterPlayerController* Pc;
};
