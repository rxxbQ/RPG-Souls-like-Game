// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter.h"
#include "Components/WidgetComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/AIController/BaseAIController.h"
#include "AI/AIController/BossAIController.h"
#include "Kismet/GameplayStatics.h"
#include "AI/UI/HealthBar.h"
#include "Inventory/WeaponItemActor.h"
#include "Inventory/BladeWeapon.h"
#include "RPG_Souls_likeCharacter.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "RPG_Souls_likeCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/BlackBoard/BlackboardKeys.h"

// Sets default values
ABossCharacter::ABossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ABossAIController::StaticClass();

	SkeletalMesh = CreateDefaultSubobject<USkeletalMesh>(TEXT("SkeletalMesh"));

	SkeletalMesh = LoadObject<USkeletalMesh>(NULL, TEXT("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Warrior.SK_CharM_Warrior'"));

	GetMesh()->SetSkeletalMesh(SkeletalMesh);

	GetCharacterMovement()->MaxWalkSpeed = 300;

	//load anim instance
	UClass* AnimationClass = LoadObject<UClass>(NULL, TEXT("AnimBlueprint'/Game/AI/Boss/Animation/standing_melee_AnimBP.standing_melee_AnimBP_C'"));
	if (!AnimationClass) return;

	GetMesh()->SetAnimInstanceClass(AnimationClass);

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	Health = MaxHealth;

	Exp = 3000;

	AttackDefence = 0.85f;

	Endurance = 4;

	AbleToAttack = true;

	BattleStart = false;

	LockOnTargetWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOn"));

	if (LockOnTargetWidget) {
		LockOnTargetWidget->SetupAttachment(RootComponent);
		LockOnTargetWidget->SetWidgetSpace(EWidgetSpace::Screen);
		LockOnTargetWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 40.0f));
		LockOnTargetWidget->SetVisibility(false);

		static ConstructorHelpers::FClassFinder<UUserWidget> LockOnTargetWidgetClass(TEXT("WidgetBlueprint'/Game/AI/UI/LockOnTargetUI.LockOnTargetUI_C'"));
		if (LockOnTargetWidgetClass.Succeeded()) {
			LockOnTargetWidget->SetWidgetClass(LockOnTargetWidgetClass.Class);
		}
	}

	// load attack combo_1 animation montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackComboMontage_1Object(TEXT("AnimMontage'/Game/AI/Boss/Animation/Attack1_Montage.Attack1_Montage'"));
	if (AttackComboMontage_1Object.Succeeded()) {
		AttackComboMontage_1 = AttackComboMontage_1Object.Object;
	}

	// load attack combo_2 animation montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackComboMontage_2Object(TEXT("AnimMontage'/Game/AI/Boss/Animation/Attack2_Montage.Attack2_Montage'"));
	if (AttackComboMontage_2Object.Succeeded()) {
		AttackComboMontage_2 = AttackComboMontage_2Object.Object;
	}

	// load attack combo_3 animation montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackComboMontage_3Object(TEXT("AnimMontage'/Game/AI/Boss/Animation/Attack3_Montage.Attack3_Montage'"));
	if (AttackComboMontage_3Object.Succeeded()) {
		AttackComboMontage_3 = AttackComboMontage_3Object.Object;
	}

	// load hit reaction animation montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ReactMontageObject(TEXT("AnimMontage'/Game/AI/Boss/Animation/React_Montage.React_Montage'"));
	if (ReactMontageObject.Succeeded()) {
		ReactMontage = ReactMontageObject.Object;
	}

	//load second stage animation montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> BattlecryMontageObject(TEXT("AnimMontage'/Game/AI/Boss/Animation/Battlecry_Montage.Battlecry_Montage'"));
	if (BattlecryMontageObject.Succeeded()) {
		BattlecryMontage = BattlecryMontageObject.Object;
	}

	//load death animation montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObject(TEXT("AnimMontage'/Game/AI/Boss/Animation/Death_Montage.Death_Montage'"));
	if (DeathMontageObject.Succeeded()) {
		DeathMontage = DeathMontageObject.Object;
	}

	SecondStage = false;
}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();

	ABossCharacter::SpawnWeapon();

	if (Weapon) {
		Weapon->WeaponCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnAttackOverlapBegin);
		Weapon->WeaponCollisionBox->OnComponentEndOverlap.AddDynamic(this, &ABossCharacter::OnAttackOverlapEnd);
	}

	if (BossHealthWidgetClass != nullptr) {
		BossHealthWidget = CreateWidget(GetWorld(), BossHealthWidgetClass);
	}
	
}

// Called every frame
void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ABaseAIController* const BossController = Cast<ABaseAIController>(GetController());

	if (BossController) {
		if (BossController->GetBlackboard()->GetValueAsBool(BbKeys::CanSeePlayer) && !BattleStart) {
			BattleStart = true;
		}
	}

	if(BattleStart){
		if (!BossHealthWidget->IsInViewport()) {
			ShowHealthBar();
		}
			
		if (Health > 0) {
			ARPG_Souls_likeCharacter* const Player = Cast<ARPG_Souls_likeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			if (Player) {
				FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Player->GetActorLocation());
				FRotator TargetRotator = FRotator(GetActorRotation().Pitch, LookRotation.Yaw, GetActorRotation().Roll);
				FRotator NewRotation = UKismetMathLibrary::RInterpTo(GetActorRotation(), TargetRotator, DeltaTime, 5.0f);
				SetActorRotation(NewRotation);
			}
		}
	}

	if (BossHealthWidget) {
		if (BossHealthWidget->IsInViewport()) {
			auto const UserWidget = Cast<UHealthBar>(BossHealthWidget);
			if (UserWidget) {
				UserWidget->SetBarValuePercent(Health / MaxHealth);
			}
		}
	}
}

void ABossCharacter::ShowHealthBar()
{
	if (BossHealthWidget) {
		BossHealthWidget->AddToViewport();
	}
}

uint32 ABossCharacter::GetExp() const
{
	return Exp;
}

float ABossCharacter::GetHealth() const
{
	return Health;
}

float ABossCharacter::GetAttackDefence() const
{
	return AttackDefence;
}

float ABossCharacter::GetMaxHealth() const
{
	return MaxHealth;
}

void ABossCharacter::SetHealth(float const NewHealth)
{
	Health = NewHealth;

	if (Health <= (MaxHealth / 2) && !SecondStage) {
		SecondStage = true;
		GetCharacterMovement()->MaxWalkSpeed = 600;
		PlayAnimMontage(BattlecryMontage, 1.0f, TEXT("Default"));
	}

	if (Health <= 0) {
		PlayAnimMontage(DeathMontage, 1.0f, TEXT("Default"));
		GetWorld()->GetTimerManager().SetTimer(DeathTimeHandler, this, &ABossCharacter::Death, 4.5f, false);
	}

	Endurance--;

	if (Endurance == 0) {
		if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(BattlecryMontage)) {
			HitReaction();
		}
		Endurance = 4;
	}
}

void ABossCharacter::AttackStart()
{
	Weapon->WeaponCollisionBox->SetCollisionProfileName("EnemyWeapon");
	Weapon->WeaponCollisionBox->SetNotifyRigidBodyCollision(true);
}

void ABossCharacter::AttackEnd()
{
	Weapon->WeaponCollisionBox->SetCollisionProfileName("NoCollision");
	Weapon->WeaponCollisionBox->SetNotifyRigidBodyCollision(false);
}

void ABossCharacter::SuperArmorStart()
{
	GetCharacterMovement()->DisableMovement();
	SuperArmor = true;
}

void ABossCharacter::SuperArmorEnd()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	SuperArmor = false;
}

void ABossCharacter::MeleeAttack()
{
	int32 randNum;
	if (SecondStage) {
		randNum = FMath::RandRange(0, 2);
	}
	else {
		randNum = FMath::RandRange(0, 1);
	}
	
	switch (randNum)
	{
	case 0:
		if (AttackComboMontage_1 && AbleToAttack) {
			AttackMontage = AttackComboMontage_1;
			UE_LOG(LogTemp, Warning, TEXT("MeleeAttack1"));
			PlayAnimMontage(AttackMontage, 1.0f, TEXT("Default"));
		}
		break;
	case 1:
		if (AttackComboMontage_3 && AbleToAttack) {
			AttackMontage = AttackComboMontage_3;
			UE_LOG(LogTemp, Warning, TEXT("MeleeAttack3"));
			PlayAnimMontage(AttackMontage, 1.0f, TEXT("Default"));
		}
		break;
	case 2:
		if (AttackComboMontage_2 && AbleToAttack) {
			AttackMontage = AttackComboMontage_2;
			UE_LOG(LogTemp, Warning, TEXT("MeleeAttack2"));
			PlayAnimMontage(AttackMontage, 1.0f, TEXT("Default"));
		}
		break;
	}
}

void ABossCharacter::HitReaction()
{
	if (!SuperArmor || Endurance <= 0) {
		AbleToAttack = false;
		PlayAnimMontage(ReactMontage, 1.0f, TEXT("Default"));
	}
}

void ABossCharacter::OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!AlreadyDamagedPlayer.Contains(OtherActor)) {
		if (ARPG_Souls_likeCharacter* const Player = Cast<ARPG_Souls_likeCharacter>(OtherActor)) {
			if (Player->AttackBlocked == false) {
				int32 const NewHealth = Player->GetCharacterProperty().CharacterCurrentHp - (Player->GetCharacterProperty().CharacterMaxHp * 0.4 + 10);
				Player->SetHealth(NewHealth);

				AlreadyDamagedPlayer.Add(OtherActor);

			}
			else {
				float const NewStamina = Player->GetCharacterProperty().CharacterCurrentStamina - 30;
				Player->SetStamina(NewStamina);

				int32 const NewHealth = Player->GetCharacterProperty().CharacterCurrentHp - (Player->GetCharacterProperty().CharacterMaxHp * 0.4 + 10) * 0.59;
				Player->SetHealth(NewHealth);
			}
			Player->GettingHit();
		}
	}
}

void ABossCharacter::OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AlreadyDamagedPlayer.Empty();
}

void ABossCharacter::ToggleLockOnTargetWidget(bool IsTargeted)
{
	if (IsTargeted) {
		LockOnTargetWidget->SetVisibility(true);
	}
	else {
		LockOnTargetWidget->SetVisibility(false);
	}
}

UAnimMontage* ABossCharacter::GetAttackMontage() const
{
	return AttackMontage;
}

void ABossCharacter::SpawnWeapon()
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
			Weapon->SetupWeapon(FName("AnthraciteAxe"));
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("weapon_socket"));
			Weapon->MeshComponent->SetRelativeLocation(FVector(0, 0, 0));
			Weapon->WeaponCollisionBox->SetCollisionProfileName("NoCollision");
			Weapon->Pickable = false;
		}
	}
}

void ABossCharacter::Death()
{
	BattleStart = false;

	if (BossHealthWidget) {
		BossHealthWidget->RemoveFromParent();
	}

	ARPG_Souls_likeCharacter* const Ch = Cast<ARPG_Souls_likeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	uint32 NewExp = Ch->GetCharacterProperty().CharacterCurrentExp + Exp;
	Ch->SetExp(NewExp);
	
	Destroy();
	Weapon->Destroy();
}