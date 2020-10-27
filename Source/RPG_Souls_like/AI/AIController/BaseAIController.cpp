// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAIController.h"
//#include "RPG_Souls_like/AI/BlackBoard/BaseBlackboardData.h"
//#include "RPG_Souls_like/AI/BehaviorTree/BaseBehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "RPG_Souls_like/AICharacter.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "RPG_Souls_like/AI/Blackboard/BlackboardKeys.h"
#include "Engine/World.h"
#include "kismet/GameplayStatics.h"
#include "RPG_Souls_like/RPG_Souls_likeCharacter.h"
#include "RPG_Souls_like/AI/AITags.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

ABaseAIController::ABaseAIController(FObjectInitializer const& ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> Obj(TEXT("BehaviorTree'/Game/AI/Behavior/BaseBT.BaseBT'"));

	if (Obj.Succeeded())
	{
		BehaviorTree = Obj.Object;
	}

	BehaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTreeComp"));
	BlackboardComponent = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	SetupPerceptionSystem();

}

void ABaseAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);
	BehaviorTreeComponent->StartTree(*BehaviorTree);
}

UBlackboardComponent* ABaseAIController::GetBlackboard() const
{
	return BlackboardComponent;
}

void ABaseAIController::OnPossess(APawn* const InPawn)
{
	Super::OnPossess(InPawn);

	if (Blackboard) {
		BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	}

	/*
	AAICharacter* AICharacter = Cast<AAICharacter>(InPawn);
	if (AICharacter) {
		Blackboard = NewObject<UBaseBlackboardData>();
		if (Blackboard) {
			UseBlackboard(Blackboard, BlackboardComponent);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Blackboard cast failed"));
		}
		BehaviorTree = NewObject<UBaseBehaviorTree>();
		if (BehaviorTree) {
			BehaviorTreeComponent->StartTree(*BehaviorTree);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("BehaviorTree cast failed"));
		}

	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("InPawn cast failed"));
	}
	*/
}

void ABaseAIController::OnUnPossess()
{
	BehaviorTreeComponent->StopTree();
}

/*
void ABaseAIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto const Ch = Cast<ARPG_Souls_likeCharacter>(Actor)) {
		GetBlackboard()->SetValueAsBool(BbKeys::CanSeePlayer, Stimulus.WasSuccessfullySensed());
	}
}
*/

void ABaseAIController::OnUpdated(TArray<AActor*> const& UpdatedActors)
{
	for (size_t x = 0; x < UpdatedActors.Num(); ++x) {
		FActorPerceptionBlueprintInfo Info;
		GetPerceptionComponent()->GetActorsPerception(UpdatedActors[x], Info);

		for (size_t k = 0; k < Info.LastSensedStimuli.Num(); ++k) {
			FAIStimulus const Stim = Info.LastSensedStimuli[k];

			if (Stim.Tag == Tags::NoiseTag) {
				GetBlackboard()->SetValueAsBool(BbKeys::IsInvestigating, Stim.WasSuccessfullySensed());
				GetBlackboard()->SetValueAsVector(BbKeys::TargetLocation, Stim.StimulusLocation);
			}
			else if (Stim.Type.Name == "Default__AISense_Sight") {
				GetBlackboard()->SetValueAsBool(BbKeys::CanSeePlayer, Stim.WasSuccessfullySensed());
			}
		}
	}

}

void ABaseAIController::SetupPerceptionSystem()
{
	//create and initialize sight configuration object
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	if (SightConfig) {
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

		SightConfig->SightRadius = 500.0f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.0f;
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;
		SightConfig->SetMaxAge(5.0f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.0f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		//add sight configuration component to perception component
		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		//GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseAIController::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}

	//create and initialize hearing config object
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));

	if (HearingConfig) {
		HearingConfig->HearingRange = 3000.0f;
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

		//add hearing configuration component to perception component
		GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &ABaseAIController::OnUpdated);
		GetPerceptionComponent()->ConfigureSense(*HearingConfig);
	}
}
