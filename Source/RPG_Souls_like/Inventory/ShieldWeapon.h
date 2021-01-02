// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG_Souls_like/Inventory/WeaponItemActor.h"
#include "ShieldWeapon.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SOULS_LIKE_API AShieldWeapon : public AWeaponItemActor
{
	GENERATED_BODY()

public:
	AShieldWeapon();

	//collision box
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* ShieldCollisionBox;
	
};
