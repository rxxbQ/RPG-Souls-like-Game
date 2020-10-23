// Fill out your copyright notice in the Description page of Project Settings.


#include "PaladinCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Type/StructType.h"
#include "Engine/Texture2D.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "StatusHUD.h"
#include "Kismet/GameplayStatics.h"

APaladinCharacter::APaladinCharacter() 
{
	PaladinSkeletalMesh = CreateDefaultSubobject<USkeletalMesh>(TEXT("PaladinSkeletalMesh"));

	//load object
	PaladinSkeletalMesh = LoadObject<USkeletalMesh>(NULL, TEXT("SkeletalMesh'/Game/Assets/Character/Paladin_Character.Paladin_Character'"));

	//set model
	GetMesh()->SetSkeletalMesh(PaladinSkeletalMesh);

	//load animation instance
	static ConstructorHelpers::FClassFinder<UAnimInstance> PaladinAnim(TEXT("AnimBlueprint'/Game/Assets/AnimBP/Paladin_BP.Paladin_BP_C'"));
	if (PaladinAnim.Class) {
		GetMesh()->SetAnimInstanceClass(PaladinAnim.Class);
	}

	//load attack animation montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageObject(TEXT("AnimMontage'/Game/Assets/AnimBP/Attack_Montage.Attack_Montage'"));
	if (AttackMontageObject.Succeeded()) {
		AttackMontage = AttackMontageObject.Object;
	}

	//load spell cast animation montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> CastSpellMontageObject(TEXT("AnimMontage'/Game/Assets/AnimBP/CastSpell_Montage.CastSpell_Montage'"));
	if (CastSpellMontageObject.Succeeded()) {
		CastSpellMontage = CastSpellMontageObject.Object;
	}

	//load block animation montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> BlockMontageObject(TEXT("AnimMontage'/Game/Assets/AnimBP/Block_Montage.Block_Montage'"));
	if (BlockMontageObject.Succeeded()) {
		BlockMontage = BlockMontageObject.Object;
	}

	// load spell cast particle system
	static ConstructorHelpers::FObjectFinder<UParticleSystem> MyParticleSystem(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Ability/Heal/P_TerminusHeal.P_TerminusHeal'"));
	if (MyParticleSystem.Succeeded()) {
		SpellParticle = MyParticleSystem.Object;
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

	CharacterAttribute.CharacterMaxStamina = 100.0f;

	CharacterAttribute.CharacterCurrentStamina = 100.0f;

	CharacterAttribute.CharacterMaxExp = 758;

	CharacterAttribute.CharacterCurrentExp = 0;

	CharacterAttribute.CharacterMovementSpeed = GetCharacterMovement()->MaxWalkSpeed;

	CharacterAttribute.CharacterAttackDamage = 0;

	CharacterAttribute.CharacterMagicDamage = 0;

}

void APaladinCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerStatusWidgetClass != nullptr) {
		PlayerStatusWidget = CreateWidget(GetWorld(), PlayerStatusWidgetClass);

		PlayerStatusWidget->AddToViewport();
	}
}

//called every frame
void APaladinCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto const StatusWidget = Cast<UStatusHUD>(PlayerStatusWidget);

	if (StatusWidget) {
		StatusWidget->SetHealthValuePercent(float(CharacterAttribute.CharacterCurrentHp) / float(CharacterAttribute.CharacterMaxHp));
		StatusWidget->SetManaValuePercent(float(CharacterAttribute.CharacterCurrentMp) / float(CharacterAttribute.CharacterMaxMp));
		StatusWidget->SetStaminaValuePercent(float(CharacterAttribute.CharacterCurrentStamina) / float(CharacterAttribute.CharacterMaxStamina));
	}

	if (CharacterAttribute.CharacterCurrentStamina < CharacterAttribute.CharacterMaxStamina && RegenerateStamina == true) {
		CharacterAttribute.CharacterCurrentStamina += 0.1f;
	}
	
	//CharacterAttribute.CharacterCurrentHp -= DeltaTime * 0.3f;
}

void APaladinCharacter::AttackStart()
{
	ARPG_Souls_likeCharacter::AttackStart();
}

void APaladinCharacter::AttackEnd()
{
	ARPG_Souls_likeCharacter::AttackEnd();
	//PlayAnimMontage(AttackMontage, 1.0f, FName("end_1"));
}

void APaladinCharacter::AttackInput()
{
	if (CharacterAttribute.CharacterCurrentStamina > 0) {
		
		int32 const NewStamina = CharacterAttribute.CharacterCurrentStamina - 20;
		SetStamina(NewStamina);

		//generate a random number between 1 and 3
		int MontageSectionIndex = rand() % 2 + 1;

		//fstring animation section
		FString MontageSection = "start_" + FString::FromInt(MontageSectionIndex);

		PlayAnimMontage(AttackMontage, 1.0f, FName(*MontageSection));

	}

	
}

void APaladinCharacter::CastSpellStart()
{
	ARPG_Souls_likeCharacter::CastSpellStart();
}

void APaladinCharacter::CastSpellEnd()
{
	ARPG_Souls_likeCharacter::CastSpellEnd();
}

void APaladinCharacter::CastSpellInput()
{
	
	if (CharacterAttribute.CharacterCurrentMp > 0) {

		int32 const NewMp = CharacterAttribute.CharacterCurrentMp - 65;
		int32 const NewHealth = CharacterAttribute.CharacterCurrentHp + 891;
		SetMana(NewMp);
		SetHealth(NewHealth);

		//ParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpellParticle, GetActorLocation(), GetActorRotation());
		ParticleComponent = UGameplayStatics::SpawnEmitterAttached(SpellParticle, GetMesh(), "right_toe_socket", FVector(-30.0f, 0.0f, -60.0f));

		//section 1
		//int MontageSectionIndex = 1;

		//fstring animation section
		FString MontageSection = "Default";

		PlayAnimMontage(CastSpellMontage, 1.0f, FName(*MontageSection));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Not Enough Mana"));
	}
	
}

void APaladinCharacter::BlockStart()
{
	ARPG_Souls_likeCharacter::BlockStart();
}

void APaladinCharacter::BlockEnd()
{
	ARPG_Souls_likeCharacter::BlockEnd();
}

void APaladinCharacter::BlockInput()
{
	//fstring animation section
	FString MontageSection = "start_1";

	PlayAnimMontage(BlockMontage, 1.0f, FName(*MontageSection));
}

