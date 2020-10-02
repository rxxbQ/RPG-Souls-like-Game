// Fill out your copyright notice in the Description page of Project Settings.


#include "PaladinCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Type/StructType.h"
#include "Engine/Texture2D.h"
#include "GameFramework/CharacterMovementComponent.h"

APaladinCharacter::APaladinCharacter() 
{
	PaladinSkeletalMesh = CreateDefaultSubobject<USkeletalMesh>(TEXT("PaladinSkeletalMesh"));

	//load object
	PaladinSkeletalMesh = LoadObject<USkeletalMesh>(NULL, TEXT("SkeletalMesh'/Game/Assets/Character/Paladin_Character.Paladin_Character'"));

	//set model
	GetMesh()->SetSkeletalMesh(PaladinSkeletalMesh);

	static ConstructorHelpers::FClassFinder<UAnimInstance> PaladinAnim(TEXT("AnimBlueprint'/Game/Assets/AnimBP/Paladin_BP.Paladin_BP_C'"));
	if (PaladinAnim.Class) {
		GetMesh()->SetAnimInstanceClass(PaladinAnim.Class);
	}

	/* initialize attribute*/
	CharacterAttribute.CharacterClass = ECharacterClass::Warrior;

	CharacterAttribute.CharacterImage = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Assets/Character/Paladin.Paladin'"));

	CharacterAttribute.CharacterLevel = 7;

	CharacterAttribute.CharacterVitality = 14;

	CharacterAttribute.CharacterAttunement = 6;

	CharacterAttribute.CharacterStrength = 16;

	CharacterAttribute.CharacterAgility = 9;

	CharacterAttribute.CharacterIntelligence = 9;

	CharacterAttribute.CharacterEndurance = 12;

	CharacterAttribute.CharacterResistance = 0.12f;

	CharacterAttribute.CharacterMaxHp = 500;

	CharacterAttribute.CharacterCurrentHp = 500;

	CharacterAttribute.CharacterMaxMp = 100;

	CharacterAttribute.CharacterCurrentMp = 100;

	CharacterAttribute.CharacterMaxStamina = 100;

	CharacterAttribute.CharacterCurrentStamina = 100;

	CharacterAttribute.CharacterMaxExp = 758;

	CharacterAttribute.CharacterCurrentExp = 0;

	CharacterAttribute.CharacterMovementSpeed = GetCharacterMovement()->MaxWalkSpeed;

	//CharacterAttribute.CharacterAttackDamage;

	//CharacterAttribute.CharacterMagicDamage;
	
}
