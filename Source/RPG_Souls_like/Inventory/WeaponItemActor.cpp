// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponItemActor.h"

AWeaponItemActor::AWeaponItemActor() 
{
	ItemAllType.ItemType = E_Weapon;
	ItemAllType.CanOverlap = false;
	ItemAllType.CanUse = false;

	//load item info
	LoadItemInfo(TEXT("1"));
}

bool AWeaponItemActor::UseItem()
{
	UE_LOG(LogTemp, Warning, TEXT("Used weapon"));
	return true;
}
