// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "AI/AIController/BaseAIController.h"
#include "AI/AIController/EnemyAIController.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "AI/UI/HealthBar.h"
#include "Components/BoxComponent.h"
#include "RPG_Souls_likeCharacter.h"
#include "RPG_Souls_like/Inventory/WeaponItemActor.h"
#include "RPG_Souls_like/Inventory/BladeWeapon.h"
#include "RPG_Souls_like/Inventory/ConsumableItemActor.h"
#include "Kismet/GameplayStatics.h"
#include "RPG_Souls_like/AI/Blackboard/BlackboardKeys.h"
#include "Kismet/KismetMathLibrary.h"

//#include "RPG_Souls_like/AI/BlackBoard/BaseBlackboardData.h"

AAICharacter::AAICharacter()
{
	AIControllerClass = AEnemyAIController::StaticClass();

	SkeletalMesh = CreateDefaultSubobject<USkeletalMesh>(TEXT("SkeletalMesh"));

	SkeletalMesh = LoadObject<USkeletalMesh>(NULL, TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));

	GetMesh()->SetSkeletalMesh(SkeletalMesh);

	bUseControllerRotationYaw = false;
	//GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//SensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("SensingComponent"));
	//SensingComponent->OnSeePawn.AddDynamic(this, &AAICharacter::SeePawn);

	Health = MaxHealth;

	Exp = 30;

	AbleToAttack = true;

	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));

	if (HealthWidgetComp) {
		HealthWidgetComp->SetupAttachment(RootComponent);
		HealthWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
		HealthWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));

		static ConstructorHelpers::FClassFinder<UUserWidget> HealthWidgetClass(TEXT("WidgetBlueprint'/Game/AI/UI/HealthBar_BP.HealthBar_BP_C'"));
		if (HealthWidgetClass.Succeeded()) {
			HealthWidgetComp->SetWidgetClass(HealthWidgetClass.Class);
		}
	}

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

	/*
	RightFistCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightFistCollisionBox"));

	if (RightFistCollisionBox) {
		FVector const extent(8.0f);
		RightFistCollisionBox->SetBoxExtent(extent, false);
		RightFistCollisionBox->SetCollisionProfileName("NoCollision");
	}
	*/

	// load attack animation montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageObject(TEXT("AnimMontage'/Game/AI/Animation/UE4_Attack_Montage.UE4_Attack_Montage'"));
	if (AttackMontageObject.Succeeded()) {
		AttackMontage = AttackMontageObject.Object;
	}

	// load execution animation montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ExecutionMontageObject(TEXT("AnimMontage'/Game/AI/Animation/Brutal_Assassination_Victim_Montage.Brutal_Assassination_Victim_Montage'"));
	if (ExecutionMontageObject.Succeeded()) {
		ExecutionMontage = ExecutionMontageObject.Object;
	}

	//load hit reaction animation montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitReactionMontageObject(TEXT("AnimMontage'/Game/AI/Animation/HitReaction_Montage.HitReaction_Montage'"));
	if (HitReactionMontageObject.Succeeded()) {
		HitReactionMontage = HitReactionMontageObject.Object;
	}
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	AAICharacter::SpawnWeapon();

	if (Weapon) {
		Weapon->WeaponCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AAICharacter::OnAttackOverlapBegin);
		Weapon->WeaponCollisionBox->OnComponentEndOverlap.AddDynamic(this, &AAICharacter::OnAttackOverlapEnd);
	}
	/*
	if (RightFistCollisionBox) {
		FAttachmentTransformRules const Rules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			false
			);
		RightFistCollisionBox->AttachToComponent(GetMesh(), Rules, "hand_r_socket");
		RightFistCollisionBox->SetRelativeLocation(FVector(-7.0f, 0.0f, 0.0f));
	}

	//attach delegates to the collision box
	if (RightFistCollisionBox) {
		RightFistCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AAICharacter::OnAttackOverlapBegin);
		RightFistCollisionBox->OnComponentEndOverlap.AddDynamic(this, &AAICharacter::OnAttackOverlapEnd);
	}
	*/
}

void AAICharacter::Tick(float const DeltaTime)
{
	Super::Tick(DeltaTime);

	auto const UserWidget = Cast<UHealthBar>(HealthWidgetComp->GetUserWidgetObject());
	if (UserWidget) {
		UserWidget->SetBarValuePercent(Health / MaxHealth);
	}

	if (Health > 0) {
		ABaseAIController* AIController = Cast<ABaseAIController>(GetController());
		if (AIController->GetBlackboard()->GetValueAsBool(BbKeys::CanSeePlayer)) {
			ARPG_Souls_likeCharacter* const Player = Cast<ARPG_Souls_likeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			if (Player) {
				FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Player->GetActorLocation());
				FRotator TargetRotation = FRotator(GetActorRotation().Pitch, LookRotation.Yaw, GetActorRotation().Roll);
				FRotator NewRotation = UKismetMathLibrary::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 5.0f);
				SetActorRotation(NewRotation);
			}
		}
	}

}

APatrolPath* AAICharacter::GetPatrolPath()
{
	return PatrolPath;
}

/*
int AAICharacter::MeleeAttack_Implementation()
{
	if (Montage) {
		PlayAnimMontage(Montage);
	}
	return 0;
}
*/

void AAICharacter::MeleeAttack()
{
	if (AttackMontage && AbleToAttack) {
		PlayAnimMontage(AttackMontage);
	}
}

UAnimMontage* AAICharacter::GetAttackMontage() const
{
	return AttackMontage;
}

float AAICharacter::GetHealth() const
{
	return Health;
}

float AAICharacter::GetMaxHealth() const
{
	return MaxHealth;
}

uint32 AAICharacter::GetExp() const
{
	return Exp;
}

void AAICharacter::SetHealth(float const NewHealth)
{
	Health = NewHealth;

	if (Health <= 0) {
		ExecuteRadgoll();

		GetWorld()->GetTimerManager().SetTimer(DeathTimeHandler, this, &AAICharacter::Death, 2.f, false);
	}
}

void AAICharacter::AttackStart()
{
	//RightFistCollisionBox->SetCollisionProfileName("EnemyWeapon");
	//RightFistCollisionBox->SetNotifyRigidBodyCollision(true);
	Weapon->WeaponCollisionBox->SetCollisionProfileName("EnemyWeapon");
	Weapon->WeaponCollisionBox->SetNotifyRigidBodyCollision(true);
}

void AAICharacter::AttackEnd()
{
	//RightFistCollisionBox->SetCollisionProfileName("NoCollision");
	//RightFistCollisionBox->SetNotifyRigidBodyCollision(false);
	Weapon->WeaponCollisionBox->SetCollisionProfileName("NoCollision");
	Weapon->WeaponCollisionBox->SetNotifyRigidBodyCollision(false);
}

bool AAICharacter::CanBeAssassinated()
{
	ABaseAIController* AIController = Cast<ABaseAIController>(GetController());
	
	if (AIController) {
		bool CanBeAssassinated = AIController->GetBlackboard()->GetValueAsBool(BbKeys::CanSeePlayer);
		return !CanBeAssassinated;
	}
	return false;
}

void AAICharacter::ExecuteStealth()
{
	ABaseAIController* const AIController = Cast<ABaseAIController>(GetController());
	AIController->OnUnPossess();
	GetCharacterMovement()->DisableMovement();
	PlayAnimMontage(ExecutionMontage, 1.0f, TEXT("Default"));

	GetWorld()->GetTimerManager().SetTimer(DeathTimeHandler, this, &AAICharacter::Death, 4.5f, false);

}

void AAICharacter::SetPatrolPath(APatrolPath* const Path)
{
	PatrolPath = Path;
}

void AAICharacter::OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (AWeaponItemActor* const Shield = Cast<AWeaponItemActor>(OtherActor)) {
	//	AttackBlocked = true;
	//	UE_LOG(LogTemp, Warning, TEXT("overlap begin: %s"), *OtherActor->GetName());
	//}
	if (!AlreadyDamagedPlayer.Contains(OtherActor)) {
		if (ARPG_Souls_likeCharacter* const Player = Cast<ARPG_Souls_likeCharacter>(OtherActor)) {
			if (Player->AttackBlocked == false) {
				int32 const NewHealth = Player->GetCharacterProperty().CharacterCurrentHp - Player->GetCharacterProperty().CharacterMaxHp * 0.2;
				Player->SetHealth(NewHealth);
				
				AlreadyDamagedPlayer.Add(OtherActor);
				UE_LOG(LogTemp, Warning, TEXT("Hit start"));
			}
			else {
				float const NewStamina = Player->GetCharacterProperty().CharacterCurrentStamina - 30;
				Player->SetStamina(NewStamina);
				UE_LOG(LogTemp, Warning, TEXT("Attack Get Blocked"));
			}
			Player->GettingHit();
		}
	}
}

void AAICharacter::OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AlreadyDamagedPlayer.Empty();
	//if (AWeaponItemActor* const Shield = Cast<AWeaponItemActor>(OtherActor)) {
	//	AttackBlocked = false;
	//}
}

void AAICharacter::HitReaction()
{
	AbleToAttack = false;
	PlayAnimMontage(HitReactionMontage, 1.0f, TEXT("Default"));
}

void AAICharacter::ToggleLockOnTargetWidget(bool IsTargeted)
{
	if (IsTargeted) {
		LockOnTargetWidget->SetVisibility(true);
	}
	else {
		LockOnTargetWidget->SetVisibility(false);
	}
	
}

void AAICharacter::SpawnWeapon()
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
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("hand_r_socket"));
			Weapon->MeshComponent->SetRelativeLocation(FVector(0, 0, 0));
			Weapon->WeaponCollisionBox->SetCollisionProfileName("NoCollision");
			Weapon->Pickable = false;
		}
	}
}

void AAICharacter::SpawnLoot()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;


	if (ConsumableClass) {
		FTransform ConsumableTransform;
		ConsumableTransform.SetLocation(GetMesh()->GetSocketTransform(TEXT("foot_r")).GetLocation());

		Consumable = GetWorld()->SpawnActor<AConsumableItemActor>(ConsumableClass, ConsumableTransform, SpawnParams);

		if (Consumable) {
			Consumable->Pickable = true;
		}
		
	}
}

void AAICharacter::ExecuteRadgoll()
{
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetAllBodiesBelowSimulatePhysics("pelvis", true, true);
}

void AAICharacter::Death()
{
	ARPG_Souls_likeCharacter* const Ch = Cast<ARPG_Souls_likeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	uint32 NewExp = Ch->GetCharacterProperty().CharacterCurrentExp + Exp;
	Ch->SetExp(NewExp);
	//SpawnLoot();
	Destroy();
	Weapon->Destroy();
}

/*
void AAICharacter::SeePawn(APawn* Pawn)
{
	ARPG_Souls_likeCharacter* PlayerCharacter = Cast<ARPG_Souls_likeCharacter>(Pawn);
	if (PlayerCharacter) {
		ABaseAIController* AIController = Cast<ABaseAIController>(GetController());
		if (AIController) {
			AIController->MoveToActor(PlayerCharacter);

			AIController->GetBlackboardComponent()->SetValueAsVector("EnemyLocation", GetActorLocation());
			UE_LOG(LogTemp, Warning, TEXT("%f,%f,%f"), AIController->GetBlackboardComponent()->GetValueAsVector("EnemyLocation").X ,
				AIController->GetBlackboardComponent()->GetValueAsVector("EnemyLocation").Y,
				AIController->GetBlackboardComponent()->GetValueAsVector("EnemyLocation").Z);
		}
	}
}
*/