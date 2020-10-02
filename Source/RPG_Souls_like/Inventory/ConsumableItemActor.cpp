// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsumableItemActor.h"

AConsumableItemActor::AConsumableItemActor() 
{
	ItemAllType.ItemType = E_Consumable;
	ItemAllType.CanOverlap = true;
	ItemAllType.CanUse = true;

	//load item info
	LoadItemInfo(TEXT("100"));
}

bool AConsumableItemActor::UseItem()
{
	UE_LOG(LogTemp, Warning, TEXT("Used consumable"));
	return true;
}
