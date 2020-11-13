// Fill out your copyright notice in the Description page of Project Settings.


#include "PaladinCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Type/StructType.h"
#include "Engine/Texture2D.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "StatusHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AICharacter.h"

APaladinCharacter::APaladinCharacter() 
{
	PaladinSkeletalMesh = CreateDefaultSubobject<USkeletalMesh>(TEXT("PaladinSkeletalMesh"));

	//load object
	PaladinSkeletalMesh = LoadObject<USkeletalMesh>(NULL, TEXT("SkeletalMesh'/Game/Assets/Character/Paladin_Character.Paladin_Character'"));

	//set model
	GetMesh()->SetSkeletalMesh(PaladinSkeletalMesh);

	//load animation instance
	//static ConstructorHelpers::FClassFinder<UAnimInstance> PaladinAnim(TEXT("AnimBlueprint'/Game/Assets/AnimBP/Paladin_BP.Paladin_BP_C'"));
	//if (PaladinAnim.Class) {
	//	GetMesh()->SetAnimInstanceClass(PaladinAnim.Class);
	//}

	UClass* AnimationClass = LoadObject<UClass>(NULL, TEXT("AnimBlueprint'/Game/Assets/AnimBP/Paladin_BP.Paladin_BP_C'"));
	if (!AnimationClass) return;

	GetMesh()->SetAnimInstanceClass(AnimationClass);
	
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

	//load roll animation montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> RollMontageObject(TEXT("AnimMontage'/Game/Assets/AnimBP/Rolling_Montage.Rolling_Montage'"));
	if (RollMontageObject.Succeeded()) {
		RollMontage = RollMontageObject.Object;
	}

	/*
	static ConstructorHelpers::FObjectFinder<UAnimMontage> RollbackMontageObject(TEXT("AnimMontage'/Game/Assets/AnimBP/RollBack_Montage.RollBack_Montage'"));
	if (RollbackMontageObject.Succeeded()) {
		RollbackMontage = RollbackMontageObject.Object;
	}
	*/

	// load execution animation montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ExecutionMontageObject(TEXT("AnimMontage'/Game/Assets/AnimBP/Execution_Montage.Execution_Montage'"));
	if (ExecutionMontageObject.Succeeded()) {
		ExecutionMontage = ExecutionMontageObject.Object;
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

	//CharacterAttribute.CharacterAttackDamage = Buff + Damage;

	//CharacterAttribute.CharacterMagicDamage = 0;

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

	if (CharacterAttribute.CharacterCurrentStamina < CharacterAttribute.CharacterMaxStamina && RegenerateStamina == true && CharacterAttribute.CharacterCurrentStamina > 0) {
		CharacterAttribute.CharacterCurrentStamina += 0.1f;
	}
	else if (CharacterAttribute.CharacterCurrentStamina <= 0) {
		if (!GetWorld()->GetTimerManager().IsTimerActive(StaminaTimerHandle)) {
			GetWorld()->GetTimerManager().SetTimer(StaminaTimerHandle, this, &APaladinCharacter::TriggerRegenerateStamina, 2.f, true);
		}	
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
	if (AttemptStealth()) {
		if (IsValid(GetMesh()->GetAnimInstance()->GetCurrentActiveMontage())) {
			float InRate = GetMesh()->GetAnimInstance()->GetCurrentActiveMontage()->SequenceLength / 2.0f;

			GetWorld()->GetTimerManager().SetTimer(StealthTimerHandle, this, &APaladinCharacter::ResetStealth, InRate, false);
		}
		
	}
	else if (CharacterAttribute.CharacterCurrentStamina > 0 && !GetCharacterMovement()->IsFalling() && HitRecover && !StealthKill) {
		
		if (!IsAttacking) {
			IsAttacking = true;

			switch(ComboCounter) {
			case 0:
				ComboCounter = 1;
				//PlayHighPriorityMontage(AttackMontage, 1.0f, TEXT("start_1"));
				PlayAnimMontage(AttackMontage, 1.0f, TEXT("start_1"));
				break;
			case 1:
				ComboCounter = 2;
				PlayAnimMontage(AttackMontage, 1.0f, TEXT("start_2"));
				break;
			case 2:
				ComboCounter = 0;
				PlayAnimMontage(AttackMontage, 1.0f, TEXT("start_3"));
				break;
			}

			int32 const NewStamina = CharacterAttribute.CharacterCurrentStamina - 20;
			SetStamina(NewStamina);
		}
		else {
			SaveAttack = true;
		}

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
	
	if (CharacterAttribute.CharacterCurrentMp > 0 && !GetCharacterMovement()->IsFalling() && HitRecover) {

		//ParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpellParticle, GetActorLocation(), GetActorRotation());
		ParticleComponent = UGameplayStatics::SpawnEmitterAttached(SpellParticle, GetMesh(), "right_toe_socket", FVector(-30.0f, 0.0f, -60.0f));

		//section 1
		//int MontageSectionIndex = 1;

		//fstring animation section
		FString MontageSection = "Default";

		PlayHighPriorityMontage(CastSpellMontage, 1.0f, FName(*MontageSection));

		if (SuccessPlayMontage) {
			int32 const NewMp = CharacterAttribute.CharacterCurrentMp - 65;
			int32 const NewHealth = CharacterAttribute.CharacterCurrentHp + 891;
			SetMana(NewMp);
			SetHealth(NewHealth);
		}
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
	ARPG_Souls_likeCharacter::BlockInput();

	/*
	if (CharacterAttribute.CharacterCurrentStamina > 0 && !GetCharacterMovement()->IsFalling() && HitRecover) {

		//fstring animation section
		FString MontageSection = "start_1";

		PlayHighPriorityMontage(BlockMontage, 1.0f, FName(*MontageSection));

		if (SuccessPlayMontage) {
			int32 const NewStamina = CharacterAttribute.CharacterCurrentStamina - 20;
			SetStamina(NewStamina);
		}
	}
	*/
}

void APaladinCharacter::RollStart()
{
	ARPG_Souls_likeCharacter::RollStart();
}

void APaladinCharacter::RollEnd()
{
	ARPG_Souls_likeCharacter::RollEnd();
}

void APaladinCharacter::RollInput()
{
	if (CharacterAttribute.CharacterCurrentStamina > 0 && !GetCharacterMovement()->IsFalling() && HasMovementInput() && HitRecover && !IsAttacking) {

		//fstring animation section
		FString MontageSection = "start_1";

		//SetActorRotation(GetLastMovementInputVector());
		PlayHighPriorityMontage(RollMontage, 1.0f, FName(*MontageSection));

		if (SuccessPlayMontage) {
			int32 const NewStamina = CharacterAttribute.CharacterCurrentStamina - 20;
			SetStamina(NewStamina);
		}
	}
	/*
	else if (CharacterAttribute.CharacterCurrentStamina > 0 && !GetCharacterMovement()->IsFalling() && !HasMovementInput()) {
		//fstring animation section
		FString MontageSection = "start_1";

		//PlayAnimMontage(RollMontage, 1.0f, FName(*MontageSection));
		PlayHighPriorityMontage(RollbackMontage, 1.0f, FName(*MontageSection));

		if (SuccessPlayMontage) {
			int32 const NewStamina = CharacterAttribute.CharacterCurrentStamina - 20;
			SetStamina(NewStamina);
		}
	}
	*/
}

void APaladinCharacter::TriggerRegenerateStamina()
{
	CharacterAttribute.CharacterCurrentStamina += 0.3f;
}

bool APaladinCharacter::HasMovementInput()
{
	return GetCharacterMovement()->GetLastInputVector() != FVector(0, 0, 0);
}

void APaladinCharacter::PlayHighPriorityMontage(UAnimMontage* Montage, float InPlayRate, FName StartSectionName)
{
	if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(Montage)) {
		HighPriorityMontage = Montage;
		SuccessPlayMontage = true;
		PlayAnimMontage(HighPriorityMontage, InPlayRate, StartSectionName);
	}
	else {
		SuccessPlayMontage = false;
	}
}

void APaladinCharacter::ResetCombo()
{
	ComboCounter = 0;
	SaveAttack = false;
	IsAttacking = false;
}

void APaladinCharacter::SaveComboAttack()
{
	if (SaveAttack) {

		SaveAttack = false;

		switch (ComboCounter) {
		case 0:
			ComboCounter = 1;
			//PlayHighPriorityMontage(AttackMontage, 1.0f, TEXT("start_1"));
			PlayAnimMontage(AttackMontage, 1.0f, TEXT("start_1"));
			break;
		case 1:
			ComboCounter = 2;
			PlayAnimMontage(AttackMontage, 1.0f, TEXT("start_2"));
			break;
		case 2:
			ComboCounter = 0;
			PlayAnimMontage(AttackMontage, 1.0f, TEXT("start_3"));
			break;
		}

		int32 const NewStamina = CharacterAttribute.CharacterCurrentStamina - 20;
		SetStamina(NewStamina);
	}
}

bool APaladinCharacter::AttemptStealth()
{
	if (StealthKill == false) {
		FVector End = GetActorLocation() + GetActorForwardVector() * 150.0f;
		TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
		TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);
		FHitResult OutHit;

		UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), GetActorLocation(), End, TraceObjects, false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true);

		L_HitActor = OutHit.GetActor();

		AAICharacter* const AI = Cast<AAICharacter>(L_HitActor);
		if (AI) {
			if (AI->CanBeAssassinated() && CheckEnemyDistance(L_HitActor)) {
				StealthKill = true;
				PlayHighPriorityMontage(ExecutionMontage, 1.0f, TEXT("Default"));
				AI->ExecuteStealth();

				return true;
			}
			else {
				return false;
			}
		}	
	}

	return false;
}

void APaladinCharacter::ResetStealth()
{
	StealthKill = false;
	CloseToEnemy = false;

	GetWorld()->GetTimerManager().ClearTimer(StealthTimerHandle);
}

bool APaladinCharacter::CheckEnemyDistance(AActor* ExecutionHitActor)
{
	if (IsValid(ExecutionHitActor)) {
		EnemyDistance = (GetActorLocation() - ExecutionHitActor->GetActorLocation()).Size();

		if (EnemyDistance <= 200) {
			CloseToEnemy = true;
		}
		else {
			CloseToEnemy = false;
		}
	}
	
	return CloseToEnemy;
}

