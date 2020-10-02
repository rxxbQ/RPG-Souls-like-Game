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

private:
	class ARPG_Souls_likeCharacter* BaseCharacter;

	//close UI
	UFUNCTION()
		void CloseUIEvent();

	//hp text block
	UFUNCTION()
		FText GetHealth();

};
