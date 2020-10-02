// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "WeaponItemActor.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SOULS_LIKE_API AWeaponItemActor : public AItemActor
{
	GENERATED_BODY()
	
public:
	AWeaponItemActor();
	
	virtual bool UseItem();
};
