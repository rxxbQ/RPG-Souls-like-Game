// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG_Souls_likeCharacter.h"
#include "PaladinCharacter.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SOULS_LIKE_API APaladinCharacter : public ARPG_Souls_likeCharacter
{
	GENERATED_BODY()
	
public:
	APaladinCharacter();

private:
	//Model
	class USkeletalMesh* PaladinSkeletalMesh;

};
