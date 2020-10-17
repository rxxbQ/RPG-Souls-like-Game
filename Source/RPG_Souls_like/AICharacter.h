// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "RPG_Souls_likeCharacter.h"
#include "GameFramework/Character.h"
#include "AI/Task/PatrolPath.h"
//#include "AI/CombatInterface.h"
#include "Animation/AnimMontage.h"
#include "AICharacter.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SOULS_LIKE_API AAICharacter : public ACharacter//, public ICombatInterface
{
	GENERATED_BODY()

public:
    AAICharacter();

    virtual void BeginPlay() override;

    class USkeletalMesh* SkeletalMesh;

    class UPawnSensingComponent* SensingComponent;

    UPROPERTY(EditAnywhere, Category = "AI")
        class UBehaviorTree* BehaviorTree;

    //get patrol path
    APatrolPath* GetPatrolPath();

    //int MeleeAttack_Implementation() override;

    void MeleeAttack();

    UAnimMontage* GetMontage() const;

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
        APatrolPath* PatrolPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
        UAnimMontage* Montage;
    /*
    UFUNCTION()
        void SeePawn(APawn* Pawn);
    */
};
