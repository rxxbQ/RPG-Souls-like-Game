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
	
	FTimerHandle StaminaTimerHandle;

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
};
