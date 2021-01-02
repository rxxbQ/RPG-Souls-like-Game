// Fill out your copyright notice in the Description page of Project Settings.


#include "BladeWeapon.h"

ABladeWeapon::ABladeWeapon()
{
	ItemAllType.ItemType = E_Weapon;

	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionBox"));
	WeaponCollisionBox->SetupAttachment(RootComponent);
	WeaponCollisionBox->SetHiddenInGame(false);
	WeaponCollisionBox->SetCollisionProfileName("NoCollision");
	WeaponCollisionBox->SetNotifyRigidBodyCollision(false);
}

void ABladeWeapon::SetupWeapon(FName WeaponName)
{
	AWeaponItemActor::SetupWeapon(WeaponName);

	if (WeaponDataTable) {
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(WeaponName, TEXT("Name"), true);
		if (WeaponData) {
			WeaponCollisionBox->SetBoxExtent(WeaponData->BoxExtent);
		}
	}
}
