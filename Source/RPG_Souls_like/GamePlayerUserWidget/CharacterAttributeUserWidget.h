// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterAttributeUserWidget.generated.h"

/**
 *  character attribute UI
 */
UCLASS()
class RPG_SOULS_LIKE_API UCharacterAttributeUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCharacterAttributeUserWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	//border
	class UBorder* NameBorder;

	//text
	class UTextBlock* UIName;

	//image
	class UImage* CharacterImage;

	//close button
	class UButton* CloseButton;

	/* text block*/
	class UTextBlock* CharacterClass;
	class UTextBlock* CharacterLevel;
	class UTextBlock* CharacterVitality;
	class UTextBlock* CharacterAttunement;
	class UTextBlock* CharacterMaxExp;
	class UTextBlock* CharacterCurrentExp;
	class UTextBlock* CharacterMaxHp;
	class UTextBlock* CharacterMaxMp;
	class UTextBlock* CharacterMaxStamina;
	class UTextBlock* CharacterStrength;
	class UTextBlock* CharacterAgility;
	class UTextBlock* CharacterIntelligence;
	class UTextBlock* CharacterEndurance;
	class UTextBlock* CharacterResistance;
	class UTextBlock* CharacterMovementSpeed;
	class UTextBlock* CharacterPhysics;
	class UTextBlock* CharacterMagic;

	/* level up buttons*/
	class UButton* VitalityLevelUp;
	class UButton* AttunementLevelUp;
	class UButton* StrengthLevelUp;
	class UButton* AgilityLevelUp;
	class UButton* IntelligenceLevelUp;
	class UButton* EnduranceLevelUp;
	class UButton* ResistanceLevelUp;

	UFUNCTION()
		void DisplayButton();

private:
	class ARPG_Souls_likeCharacter* BaseCharacter;

	//close UI
	UFUNCTION()
		void CloseUIEvent();

	//hp text block
	UFUNCTION()
		FText GetHealth();

	//mp text block
	UFUNCTION()
		FText GetMana();

	//stamina text block
	UFUNCTION()
		FText GetStamina();

	//level text block
	UFUNCTION()
		FText GetLevel();

	//vitality text block
	UFUNCTION()
		FText GetVitality();

	//attunement
	UFUNCTION()
		FText GetAttunement();

	//strength text block
	UFUNCTION()
		FText GetStrength();

	//agility text block
	UFUNCTION()
		FText GetAgility();

	//intelligence text block
	UFUNCTION()
		FText GetIntelligence();

	//endurance text block
	UFUNCTION()
		FText GetEndurance();

	//resistance text block
	UFUNCTION()
		FText GetResistance();

	//movement speed text block
	UFUNCTION()
		FText GetMovementSpeed();

	//max exp text block
	UFUNCTION()
		FText GetMaxExp();

	//current exp text block
	UFUNCTION()
		FText GetCurrentExp();

	//physics damage
	UFUNCTION()
		FText GetPhysics();

	//magic damage
	UFUNCTION()
		FText GetMagic();

	// Level Up Vitality event
	UFUNCTION()
		void LevelUpVitality();

	// Level Up Attunement event
	UFUNCTION()
		void LevelUpAttunement();

	// Level Up Strength event
	UFUNCTION()
		void LevelUpStrength();

	// Level Up Agility event
	UFUNCTION()
		void LevelUpAgility();

	// Level Up Intelligence event
	UFUNCTION()
		void LevelUpIntelligence();

	// Level U pEndurance event
	UFUNCTION()
		void LevelUpEndurance();

	// Level Up Resistance event
	UFUNCTION()
		void LevelUpResistance();

	void HideButton();


};
