// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBehaviorTree.h"
#include "RPG_Souls_like/AI/BlackBoard/BaseBlackboardData.h"

UBaseBehaviorTree::UBaseBehaviorTree() 
{
	//load blackboard
	BaseBlackboard = NewObject<UBaseBlackboardData>();
	if (!BaseBlackboard) {
		BlackboardAsset = BaseBlackboard;
		if (BlackboardAsset) {
			UE_LOG(LogTemp, Warning, TEXT("success"));
		}	
	}
}