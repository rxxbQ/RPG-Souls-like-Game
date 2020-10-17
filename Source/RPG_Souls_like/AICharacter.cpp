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
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
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