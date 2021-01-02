// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPG_Souls_likeCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "CharacterPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Inventory/WeaponItemActor.h"
#include "Inventory/ShieldWeapon.h"
#include "Inventory/BladeWeapon.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "AI/AITags.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "AICharacter.h"
#include "BossCharacter.h"
#include "Particles/ParticleSystemComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/WidgetComponent.h"
#include "RPG_Souls_like/GamePlayerUserWidget/CharacterAttributeUserWidget.h"

//////////////////////////////////////////////////////////////////////////
// ARPG_Souls_likeCharacter

ARPG_Souls_likeCharacter::ARPG_Souls_likeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(38.f, 83.0f);

	//initial location and rotation
	GetMesh()->SetRelativeLocation(FVector(0, -10, -83));
	GetMesh()->SetRelativeRotation(FRotator(0, 0, -90));

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	//initial movement speed
	GetCharacterMovement()->MaxWalkSpeed = 300;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	SetupStimulus();

	RegenerateStamina = false;

	TeamId = FGenericTeamId(0);

	Pc = Cast<ACharacterPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	//load normal impact anim montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ImpactMontageObject(TEXT("AnimMontage'/Game/Assets/AnimBP/Impact_Montage.Impact_Montage'"));
	if (ImpactMontageObject.Succeeded()) {
		ImpactMontage = ImpactMontageObject.Object;
	}

	// load block impact anim montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> BlockImpactMontageObject(TEXT("AnimMontage'/Game/Assets/AnimBP/BlockImpact_Montage.BlockImpact_Montage'"));
	if (BlockImpactMontageObject.Succeeded()) {
		BlockImpactMontage = BlockImpactMontageObject.Object;
	}

	// load levelup particle system
	static ConstructorHelpers::FObjectFinder<UParticleSystem> LevelParticleComponentObject(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Skill_Leap/P_Skill_Leap_Base_Charge_Weapon.P_Skill_Leap_Base_Charge_Weapon'"));
	if (LevelParticleComponentObject.Succeeded()) {
		LevelParticleSystem = LevelParticleComponentObject.Object;
	}

}

void ARPG_Souls_likeCharacter::BeginPlay()
{
	Super::BeginPlay();

	ARPG_Souls_likeCharacter::SpawnWeapon();

	//attach delegates to the collision box

	//Weapon->WeaponCollisionBox->OnComponentHit.AddDynamic(this, &ARPG_Souls_likeCharacter::OnAttackHit);

	if (Weapon->WeaponCollisionBox) {
		Weapon->WeaponCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ARPG_Souls_likeCharacter::OnAttackOverlapBegin);
		Weapon->WeaponCollisionBox->OnComponentEndOverlap.AddDynamic(this, &ARPG_Souls_likeCharacter::OnAttackOverlapEnd);
	}

	if (Shield->ShieldCollisionBox) {
		Shield->ShieldCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ARPG_Souls_likeCharacter::OnBlockOverlapBegin);
		Shield->ShieldCollisionBox->OnComponentEndOverlap.AddDynamic(this, &ARPG_Souls_likeCharacter::OnBlockOverlapEnd);
	}

	Buff = int32(Weapon->StrengthBuff * CharacterAttribute.CharacterStrength + Weapon->AgilityBuff * CharacterAttribute.CharacterAgility);
	Damage = int32(Weapon->Damage);

	CharacterAttribute.CharacterAttackDamage = Buff + Damage;
}

void ARPG_Souls_likeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ToggleLock) {
		if (LockOnTarget) {
			FVector TargetVector = FVector(LockOnTarget->GetActorLocation().X, LockOnTarget->GetActorLocation().Y, LockOnTarget->GetActorLocation().Z - 65.0f);
			FRotator StrafeRotator = UKismetMathLibrary::FindLookAtRotation(FollowCamera->GetComponentLocation(), TargetVector);
			FRotator ControllerRotator = UKismetMathLibrary::RInterpTo(GetController()->GetControlRotation(), StrafeRotator, DeltaTime, 3.0f);
			FRotator CameraRotator = FRotator( ControllerRotator.Pitch -0.2f , ControllerRotator.Yaw, GetController()->GetControlRotation().Roll);
			
			GetController()->SetControlRotation(CameraRotator);

			//TargetEnemy();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARPG_Souls_likeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARPG_Souls_likeCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARPG_Souls_likeCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ARPG_Souls_likeCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ARPG_Souls_likeCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ARPG_Souls_likeCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ARPG_Souls_likeCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ARPG_Souls_likeCharacter::OnResetVR);

	// attack functionality
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ARPG_Souls_likeCharacter::AttackInput);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &ARPG_Souls_likeCharacter::AttackEnd);

	//PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ARPG_Souls_likeCharacter::SpawnWeapon);

	PlayerInputComponent->BindAction("Distract", IE_Pressed, this, &ARPG_Souls_likeCharacter::OnDistract);

	PlayerInputComponent->BindAction("CastSpell", IE_Pressed, this, &ARPG_Souls_likeCharacter::CastSpellInput);
	PlayerInputComponent->BindAction("CastSpell", IE_Released, this, &ARPG_Souls_likeCharacter::CastSpellEnd);

	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &ARPG_Souls_likeCharacter::BlockInput);
	PlayerInputComponent->BindAction("Block", IE_Released, this, &ARPG_Souls_likeCharacter::BlockEnd);

	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &ARPG_Souls_likeCharacter::RollInput);
	PlayerInputComponent->BindAction("Roll", IE_Released, this, &ARPG_Souls_likeCharacter::RollEnd);

	PlayerInputComponent->BindAction("LockEnemy", IE_Pressed, this, &ARPG_Souls_likeCharacter::LockOnEnemy);

}

void ARPG_Souls_likeCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ARPG_Souls_likeCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ARPG_Souls_likeCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ARPG_Souls_likeCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARPG_Souls_likeCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ARPG_Souls_likeCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && HitRecover)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ARPG_Souls_likeCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) && HitRecover)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ARPG_Souls_likeCharacter::AttackStart()
{
	RegenerateStamina = false;
	GetCharacterMovement()->DisableMovement();

	Weapon->WeaponCollisionBox->SetCollisionProfileName("Weapon");
	Weapon->WeaponCollisionBox->SetNotifyRigidBodyCollision(true);

	//Weapon->WeaponCollisionBox->SetGenerateOverlapEvents(true);

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *Weapon->WeaponCollisionBox->GetCollisionProfileName().ToString());
}

void ARPG_Souls_likeCharacter::AttackEnd()
{
	RegenerateStamina = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	Weapon->WeaponCollisionBox->SetCollisionProfileName("NoCollision");
	Weapon->WeaponCollisionBox->SetNotifyRigidBodyCollision(false);
	//Weapon->WeaponCollisionBox->SetGenerateOverlapEvents(false);

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *Weapon->WeaponCollisionBox->GetCollisionProfileName().ToString());
}

void ARPG_Souls_likeCharacter::AttackInput()
{

}

void ARPG_Souls_likeCharacter::OnAttackHit(UPrimitiveComponent* HitComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("hit: %s"), *Hit.GetActor()->GetName());
}

void ARPG_Souls_likeCharacter::OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!AlreadyAttackedEnemy.Contains(OtherActor)) {
		if (AAICharacter* const AI = Cast<AAICharacter>(OtherActor)) {
			float const NewHealth = AI->GetHealth() - CharacterAttribute.CharacterAttackDamage;
			AI->SetHealth(NewHealth);
			AI->HitReaction();

			AlreadyAttackedEnemy.Add(OtherActor);
		}
		else if (ABossCharacter* const Boss = Cast<ABossCharacter>(OtherActor)) {
			float const NewHealth = Boss->GetHealth() - (CharacterAttribute.CharacterAttackDamage * Boss->GetAttackDefence());
			Boss->SetHealth(NewHealth);
			//Boss->HitReaction();
			UE_LOG(LogTemp, Warning, TEXT("attck start"));
			AlreadyAttackedEnemy.Add(OtherActor);
		}
	}
}

void ARPG_Souls_likeCharacter::OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AlreadyAttackedEnemy.Empty();
}

void ARPG_Souls_likeCharacter::OnBlockOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (AAICharacter* const AI = Cast<AAICharacter>(OtherActor)) {
		if (AWeaponItemActor* const EnemyWeapon = Cast<AWeaponItemActor>(OtherActor)) {
			AttackBlocked = true;
			//PlayAnimMontage(BlockImpactMontage, 1.0f, TEXT("Default"));
	}
}

void ARPG_Souls_likeCharacter::OnBlockOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//if (AAICharacter* const AI = Cast<AAICharacter>(OtherActor)) {
	if (AWeaponItemActor* const EnemyWeapon = Cast<AWeaponItemActor>(OtherActor)) {
		AttackBlocked = false;
	}
}

void ARPG_Souls_likeCharacter::LockOnEnemy()
{
	ToggleLock = !ToggleLock;

	if (ToggleLock) {
		IsTargeted = true;
	}
	else {
		IsTargeted = false;
	}
	TargetEnemy();
}

void ARPG_Souls_likeCharacter::TargetEnemy()
{
	FVector End = GetActorLocation() + GetActorForwardVector() * 1000.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult OutHit;
	FHitResult LineHit;

	UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), GetActorLocation(), End, 1000.0f, TraceObjects, false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true);
	
	if (AAICharacter* const AI = Cast<AAICharacter>(OutHit.GetActor())) {
		UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), AI->GetActorLocation(), UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, LineHit, true);

		if (!LineHit.bBlockingHit && ToggleLock) {
			LockOnTarget = AI;
			IsTargeted = true;
		}
		else if(LineHit.bBlockingHit && ToggleLock){
			LockOnTarget = NULL;
			IsTargeted = false;
		}
		else if (!LineHit.bBlockingHit && !ToggleLock) {
			LockOnTarget = NULL;
			IsTargeted = false;
		}
		else if (LineHit.bBlockingHit && !ToggleLock) {
			LockOnTarget = NULL;
			IsTargeted = false;
		}
		
		AI->ToggleLockOnTargetWidget(IsTargeted);
	}
	else if (ABossCharacter* const Boss = Cast<ABossCharacter>(OutHit.GetActor())) {
		UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), Boss->GetActorLocation(), UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, LineHit, true);

		if (!LineHit.bBlockingHit && ToggleLock) {
			LockOnTarget = Boss;
			IsTargeted = true;
		}
		else if (LineHit.bBlockingHit && ToggleLock) {
			LockOnTarget = NULL;
			IsTargeted = false;
		}
		else if (!LineHit.bBlockingHit && !ToggleLock) {
			LockOnTarget = NULL;
			IsTargeted = false;
		}
		else if (LineHit.bBlockingHit && !ToggleLock) {
			LockOnTarget = NULL;
			IsTargeted = false;
		}

		Boss->ToggleLockOnTargetWidget(IsTargeted);
	}
}

void ARPG_Souls_likeCharacter::CastSpellStart()
{
	//UE_LOG(LogTemp, Warning, TEXT("disable input"));
	DisableInput(Pc);
	//GetCharacterMovement()->DisableMovement();
	
}

void ARPG_Souls_likeCharacter::CastSpellEnd()
{
	//UE_LOG(LogTemp, Warning, TEXT("enable input"));
	EnableInput(Pc);
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void ARPG_Souls_likeCharacter::CastSpellInput()
{

}

void ARPG_Souls_likeCharacter::BlockStart()
{
	//RegenerateStamina = false;
	Shield->ShieldCollisionBox->SetCollisionProfileName("Shield");
	Shield->ShieldCollisionBox->SetNotifyRigidBodyCollision(true);
}

void ARPG_Souls_likeCharacter::BlockEnd()
{
	//RegenerateStamina = true;
	IsBlockPressed = false;
	CharacterState = ECharacterState::Normal;

	Shield->ShieldCollisionBox->SetCollisionProfileName("NoCollision");
	Shield->ShieldCollisionBox->SetNotifyRigidBodyCollision(false);
}

void ARPG_Souls_likeCharacter::BlockInput()
{
	IsBlockPressed = true;
	CharacterState = ECharacterState::Block;

	Shield->ShieldCollisionBox->SetCollisionProfileName("Shield");
	Shield->ShieldCollisionBox->SetNotifyRigidBodyCollision(true);
}

void ARPG_Souls_likeCharacter::RollStart()
{
	RegenerateStamina = false;
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorEnableCollision(false);
	//GetMesh()->SetCollisionProfileName("NoCollision");
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
}

void ARPG_Souls_likeCharacter::RollEnd()
{
	RegenerateStamina = true;
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetActorEnableCollision(true);
	//GetMesh()->SetCollisionProfileName("Pawn");
	GetCapsuleComponent()->SetCollisionProfileName("Pawn");
}

void ARPG_Souls_likeCharacter::RollInput()
{

}

FText ARPG_Souls_likeCharacter::GetCharacterClass()
{
	switch (GetCharacterProperty().CharacterClass.GetValue()) {
	case Assasin:
		return FText::FromString(TEXT("Class : Assasin"));
		break;

	case Mage:
		return FText::FromString(TEXT("Class : Mage"));
		break;

	case Warrior:
		return FText::FromString(TEXT("Class : Warrior"));
		break;

	case Knight:
		return FText::FromString(TEXT("Class : Knight"));
		break;

	case Deprived:
		return FText::FromString(TEXT("Class : Deprived"));
		break;

	}
	
	return FText();
}

FText ARPG_Souls_likeCharacter::GetCharacterBasic(int32 Max, int32 Current, FString String)
{
	FString NewString = String+ FString::FromInt(Current) + TEXT(" / ") + FString::FString::FromInt(Max);
	
	return FText::FromString(NewString);
}

FText ARPG_Souls_likeCharacter::GetCharacterAttribute(int32 SelfAttribute, FString String)
{
	//int32 Buff = int32(Weapon->StrengthBuff * CharacterAttribute.CharacterStrength + Weapon->AgilityBuff * CharacterAttribute.CharacterAgility);
	//int32 Damage = int32(Weapon->Damage);

	FString NewString = String + FString::FromInt(Damage) + TEXT(" + ") + FString::FromInt(Buff);
	
	return FText::FromString(NewString);
}

void ARPG_Souls_likeCharacter::SetHealth(int32 const NewHealth)
{
	if (NewHealth <= 0) {
		UE_LOG(LogTemp, Error, TEXT("You die"));

		//auto const Control = UGameplayStatics::GetPlayerController(this, 0);
		//Control->SetCinematicMode(true, false, false, true, true);
		GetMesh()->SetSimulatePhysics(true);
		GetMovementComponent()->MovementState.bCanJump = false;
		GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
	}
	else if (uint32(NewHealth) >= CharacterAttribute.CharacterMaxHp){
		CharacterAttribute.CharacterCurrentHp = CharacterAttribute.CharacterMaxHp;
	}
	else {
		CharacterAttribute.CharacterCurrentHp = NewHealth;
	}
	
}

void ARPG_Souls_likeCharacter::SetVitality(uint32 const NewVitality)
{
	CharacterAttribute.CharacterVitality = NewVitality;
	CharacterAttribute.CharacterMaxHp += 44;
	CharacterAttribute.CharacterCurrentHp += 44;
}

void ARPG_Souls_likeCharacter::SetMana(int32 const NewMana)
{
	if (NewMana <= 0) {
		CharacterAttribute.CharacterCurrentMp = 0;
	}
	else {
		CharacterAttribute.CharacterCurrentMp = NewMana;
	}
	
}

void ARPG_Souls_likeCharacter::SetAttunement(uint32 const NewAttunement)
{
	CharacterAttribute.CharacterAttunement = NewAttunement;
	CharacterAttribute.CharacterMaxMp += 10;
	CharacterAttribute.CharacterCurrentMp += 10;
}

void ARPG_Souls_likeCharacter::SetStamina(float const NewStamina)
{
	if (NewStamina <= 0) {
		CharacterAttribute.CharacterCurrentStamina = 0;
	}
	else {
		CharacterAttribute.CharacterCurrentStamina = NewStamina;
	}
}

void ARPG_Souls_likeCharacter::SetEndurance(uint32 const NewEndurance)
{
	CharacterAttribute.CharacterEndurance = NewEndurance;
	CharacterAttribute.CharacterMaxStamina += 3.0f;
}

void ARPG_Souls_likeCharacter::SetExp(uint32 const NewExp)
{
	if (NewExp >= CharacterAttribute.CharacterMaxExp) {
		LevelUp(NewExp);
	}
	else {
		CharacterAttribute.CharacterCurrentExp = NewExp;
	}
}

void ARPG_Souls_likeCharacter::SetStrength(uint32 const NewStrength)
{
	CharacterAttribute.CharacterStrength = NewStrength;
}

void ARPG_Souls_likeCharacter::SetAgility(uint32 const NewAgility)
{
	CharacterAttribute.CharacterAgility = NewAgility;
}

void ARPG_Souls_likeCharacter::SetIntelligence(uint32 const NewIntelligence)
{
	CharacterAttribute.CharacterIntelligence = NewIntelligence;
}

void ARPG_Souls_likeCharacter::SetResistance(float const NewResistance)
{
	CharacterAttribute.CharacterResistance = NewResistance;
}

void ARPG_Souls_likeCharacter::SetSkillPoint(uint8 const NewSkillPoint)
{
	CharacterAttribute.SkillPoint = NewSkillPoint;
}


void ARPG_Souls_likeCharacter::GettingHit()
{
	if (!AttackBlocked) {
		PlayAnimMontage(ImpactMontage, 1.0f, TEXT("Default"));
	}
	else {
		PlayAnimMontage(BlockImpactMontage, 1.0f, TEXT("Default"));
	}
	
}

void ARPG_Souls_likeCharacter::SpawnWeapon()
{
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (WeaponClass) {
			FTransform WeaponTransform;
			WeaponTransform.SetLocation(FVector::ZeroVector);
			WeaponTransform.SetRotation(FQuat(FRotator::ZeroRotator));

			Weapon = GetWorld()->SpawnActor<ABladeWeapon>(WeaponClass, WeaponTransform, SpawnParams);
			
			if (Weapon) {
					Weapon->SetupWeapon(FName("BlackKnight"));
					Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("s_hand_r"));
					Weapon->MeshComponent->SetRelativeLocation(FVector(0, 0, 0));
					Weapon->WeaponCollisionBox->SetCollisionProfileName("NoCollision");
					Weapon->Pickable = false;
			}
		}

		if (ShieldClass) {
			FTransform ShieldTransform;
			ShieldTransform.SetLocation(FVector::ZeroVector);
			ShieldTransform.SetRotation(FQuat(FRotator::ZeroRotator));

			Shield = GetWorld()->SpawnActor<AShieldWeapon>(ShieldClass, ShieldTransform, SpawnParams);

			if (Shield) {
				Shield->SetupWeapon(FName("Shield"));
				Shield->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("s_hand_l"));
				Shield->MeshComponent->SetRelativeLocation(FVector(0, 0, 0));
				Shield->MeshComponent->SetRelativeRotation(FQuat(FRotator(0, 0, 0)));
				Shield->ShieldCollisionBox->SetCollisionProfileName("NoCollision");
				Shield->Pickable = false;

			}
		}
		
}

void ARPG_Souls_likeCharacter::SetupStimulus()
{
	Stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	Stimulus->RegisterWithPerceptionSystem();
}

void ARPG_Souls_likeCharacter::OnDistract()
{
	if (DistractionSound) {
		FVector const Location = GetActorLocation();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DistractionSound, Location);
		UAISense_Hearing::ReportNoiseEvent(GetWorld(), Location, 1.0f, this, 0.0f, Tags::NoiseTag);
	}
}

void ARPG_Souls_likeCharacter::LevelUp(uint32 const NewExp)
{
	CharacterAttribute.CharacterLevel += 1;
	CharacterAttribute.CharacterCurrentExp = NewExp - CharacterAttribute.CharacterMaxExp;
	CharacterAttribute.CharacterMaxExp += 17;
	CharacterAttribute.SkillPoint += 1;

	LevelParticleComponent = UGameplayStatics::SpawnEmitterAttached(LevelParticleSystem, GetMesh(), "levelup_socket", FVector(0.0f, 0.0f, 0.0f));

	Pc->AttributeUserWidget->DisplayButton();
}


