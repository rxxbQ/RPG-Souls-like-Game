// Fill out your copyright notice in the Description page of Project Settings.


#include "ShieldWeapon.h"

AShieldWeapon::AShieldWeapon()
{
	ItemAllType.ItemType = E_Weapon;

	ShieldCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldCollisionBox"));
	ShieldCollisionBox->SetupAttachment(RootComponent);
	ShieldCollisionBox->SetHiddenInGame(false);
	ShieldCollisionBox->SetCollisionProfileName("NoCollision");
	ShieldCollisionBox->SetNotifyRigidBodyCollision(false);
}

