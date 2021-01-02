// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG_Souls_like/Inventory/WeaponItemActor.h"
#include "BladeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SOULS_LIKE_API ABladeWeapon : public AWeaponItemActor
{
	GENERATED_BODY()
	
public:
	ABladeWeapon();

	//collision box
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* WeaponCollisionBox;

	virtual void SetupWeapon(FName WeaponName);

};
