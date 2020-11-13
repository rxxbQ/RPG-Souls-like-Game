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

	/* cast spell montage*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowprivateAccess = "true"))
		class UAnimMontage* CastSpellMontage;

	/* block montage*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowprivateAccess = "true"))
		class UAnimMontage* BlockMontage;

	/* roll montage*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowprivateAccess = "true"))
		class UAnimMontage* RollMontage;

	/* rollback montage*/
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowprivateAccess = "true"))
	//	class UAnimMontage* RollbackMontage;
	
	/* execution montage*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowprivateAccess = "true"))
		class UAnimMontage* ExecutionMontage;

public:
	APaladinCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

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

	/**
	* CastSpellStart - trigger when the player initiates an CastSpell
	*/
	virtual void CastSpellStart();

	/**
	* CastSpellEnd - trigger when the player stops an CastSpell
	*/
	virtual void CastSpellEnd();

	/**
	* CastSpellInput - trigger CastSpell animations based on user input
	*/
	virtual void CastSpellInput();

	/**
	* BlockStart - trigger when the player initiates an Block
	*/
	virtual void BlockStart();

	/**
	* BlockEnd - trigger when the player stops an Block
	*/
	virtual void BlockEnd();

	/**
	* BlockInput - trigger Block animations based on user input
	*/
	virtual void BlockInput();

	/**
	* RollStart - trigger when the player initiates a Roll
	*/
	virtual void RollStart();

	/**
	* RollEnd - trigger when the player stops a Roll
	*/
	virtual void RollEnd();

	/**
	* RollInput - trigger Roll animations based on user input
	*/
	virtual void RollInput();
	
	FTimerHandle StaminaTimerHandle;

	FTimerHandle StealthTimerHandle;

	UFUNCTION()
		void TriggerRegenerateStamina();

private:
	//Mesh
	class USkeletalMesh* PaladinSkeletalMesh;

	UPROPERTY(EditAnywhere, Category = "UI HUD")
		TSubclassOf<UUserWidget> PlayerStatusWidgetClass;

	UUserWidget* PlayerStatusWidget;

	// cast spell particle system
	UParticleSystem* SpellParticle;

	UParticleSystemComponent* ParticleComponent;

	bool HasMovementInput();

	/* Start - play anim montage with higher priority*/
	UAnimMontage* HighPriorityMontage;

	void PlayHighPriorityMontage(UAnimMontage* Montage, float InPlayRate, FName StartSectionName);

	bool SuccessPlayMontage;
	/* End ----------------------------------*/

	/* Start - combo attacks */
	int ComboCounter;

	bool SaveAttack;

	bool IsAttacking;

	UFUNCTION(BlueprintCallable)
		void ResetCombo();

	UFUNCTION(BlueprintCallable)
		void SaveComboAttack();
	/* End ---------------------------------*/

	/* Start - assassinate enemy*/
	bool AttemptStealth();

	bool StealthKill;

	void ResetStealth();

	AActor* L_HitActor;

	bool CheckEnemyDistance(AActor* ExecutionHitActor);

	float EnemyDistance;

	float ExecutionDistance = 60.0f;

	bool CloseToEnemy;
	/* End ---------------------------------*/
};
