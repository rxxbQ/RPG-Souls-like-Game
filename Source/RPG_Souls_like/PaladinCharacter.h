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

	/* attack montage*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowprivateAccess = "true"))
	class UAnimMontage* AttackMontage;
	
public:
	APaladinCharacter();

	/**
	* AttackStart - trigger when the player initiates an attack
	*/
	virtual void AttackStart();

	/**
	* AttackEnd - trigger when the player stops an attack
	*/
	virtual void AttackEnd();

	/**
	* AttackInput - trigger attack animations based on user input
	*/
	virtual void AttackInput();

private:
	//Mesh
	class USkeletalMesh* PaladinSkeletalMesh;

};
