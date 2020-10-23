// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "AI/AIController/BaseAIController.h"
#include "Perception/PawnSensingComponent.h"
//#include "RPG_Souls_likeCharacter.h"
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

//#include "RPG_Souls_like/AI/BlackBoard/BaseBlackboardData.h"

AAICharacter::AAICharacter()
{
	AIControllerClass = ABaseAIController::StaticClass();

	SkeletalMesh = CreateDefaultSubobject<USkeletalMesh>(TEXT("SkeletalMesh"));

	SkeletalMesh = LoadObject<USkeletalMesh>(NULL, TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));

	GetMesh()->SetSkeletalMesh(SkeletalMesh);

	bUseControllerRotationYaw = false;
	//GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//SensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("SensingComponent"));
	//SensingComponent->OnSeePawn.AddDynamic(this, &AAICharacter::SeePawn);

	Health = MaxHealth;
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));

	if (WidgetComp) {
		WidgetComp->SetupAttachment(RootComponent);
		WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
		WidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 85.0f));

		static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("WidgetBlueprint'/Game/AI/UI/HealthBar_BP.HealthBar_BP_C'"));
		if (WidgetClass.Succeeded()) {
			WidgetComp->SetWidgetClass(WidgetClass.Class);
		}
	}

	RightFistCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightFistCollisionBox"));

	if (RightFistCollisionBox) {
		FVector const extent(8.0f);
		RightFistCollisionBox->SetBoxExtent(extent, false);
		RightFistCollisionBox->SetCollisionProfileName("NoCollision");
	}
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

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
}

void AAICharacter::Tick(float const DeltaTime)
{
	Super::Tick(DeltaTime);

	auto const UserWidget = Cast<UHealthBar>(WidgetComp->GetUserWidgetObject());
	if (UserWidget) {
		UserWidget->SetBarValuePercent(Health / MaxHealth);
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
	if (Montage) {
		PlayAnimMontage(Montage);
	}
}

UAnimMontage* AAICharacter::GetMontage() const
{
	return Montage;
}

float AAICharacter::GetHealth() const
{
	return Health;
}

float AAICharacter::GetMaxHealth() const
{
	return MaxHealth;
}

void AAICharacter::SetHealth(float const NewHealth)
{
	Health = NewHealth;

	if (Health <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("Enemy dies"));
		GetMesh()->SetSimulatePhysics(true);
		//Destroy();
	}
}

void AAICharacter::AttackStart()
{
	RightFistCollisionBox->SetCollisionProfileName("EnemyWeapon");
	RightFistCollisionBox->SetNotifyRigidBodyCollision(true);
}

void AAICharacter::AttackEnd()
{
	RightFistCollisionBox->SetCollisionProfileName("NoCollision");
	RightFistCollisionBox->SetNotifyRigidBodyCollision(false);
}

void AAICharacter::OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (AWeaponItemActor* const Shield = Cast<AWeaponItemActor>(OtherActor)) {
	//	AttackBlocked = true;
	//	UE_LOG(LogTemp, Warning, TEXT("overlap begin: %s"), *OtherActor->GetName());
	//}
	if (ARPG_Souls_likeCharacter* const Player = Cast<ARPG_Souls_likeCharacter>(OtherActor)) {
		if (Player->AttackBlocked == false) {
			int32 const NewHealth = Player->GetCharacterProperty().CharacterCurrentHp - Player->GetCharacterProperty().CharacterMaxHp * 0.07;
			Player->SetHealth(NewHealth);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Attack Get Blocked"));
		}
	}
}

void AAICharacter::OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//if (AWeaponItemActor* const Shield = Cast<AWeaponItemActor>(OtherActor)) {
	//	AttackBlocked = false;
	//}
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