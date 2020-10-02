// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskItemActor.h"

ATaskItemActor::ATaskItemActor() 
{
	ItemAllType.ItemType = E_Task;
	ItemAllType.CanOverlap = true;
	ItemAllType.CanUse = false;
}

bool ATaskItemActor::UseItem()
{
	return false;
}
