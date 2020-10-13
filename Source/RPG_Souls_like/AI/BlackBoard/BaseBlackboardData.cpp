// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBlackboardData.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBaseBlackboardData::UBaseBlackboardData() 
{
	Keys.SetNum(3);

	IsMove = NewObject<UBlackboardKeyType_Bool>();

	if (IsMove) {
		Keys[0].EntryName = "IsMove";
		Keys[0].KeyType = IsMove;
		Keys[0].bInstanceSynced = false;
	}

	IsIdle = NewObject<UBlackboardKeyType_Bool>();

	if (IsIdle) {
		Keys[1].EntryName = "IsIdle";
		Keys[1].KeyType = IsMove;
		Keys[1].bInstanceSynced = false;
	}

	EnemyLocation = NewObject<UBlackboardKeyType_Vector>();

	if (EnemyLocation) {
		Keys[2].EntryName = "EnemyLocation";
		Keys[2].KeyType = EnemyLocation;
		Keys[2].bInstanceSynced = false;
	}
}