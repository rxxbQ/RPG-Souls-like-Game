// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "RPG_Souls_likeCharacter.h"
#include "GameFramework/Character.h"
#include "AI/Task/PatrolPath.h"
//#include "AI/CombatInterface.h"
#include "Animation/AnimMontage.h"
#include "BehaviorTree/BehaviorTree.h"
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

    void Tick(float const DeltaTime) override;

    class USkeletalMesh* SkeletalMesh;

    class UPawnSensingComponent* SensingComponent;

    UPROPERTY(EditAnywhere, Category = "AI")
        class UBehaviorTree* BehaviorTree;

    //get patrol path
    APatrolPath* GetPatrolPath();

    //int MeleeAttack_Implementation() override;

    void MeleeAttack();

    UAnimMontage* GetMontage() const;

    float GetHealth() const;

    float GetMaxHealth() const;

    int GetExp() const;

    void SetHealth(float const NewHealth);

    virtual void AttackStart();

    virtual void AttackEnd();

    void SetPatrolPath(APatrolPath* const Path);
    /**
    * OnAttackOverlapBegin - triggerred when the collider overlaps another component
    */
    UFUNCTION()
        void OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
            int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    /**
    * OnAttackOverlapEnd - triggerred when the collider stops overlapping another component
    */
    UFUNCTION()
        void OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
        APatrolPath* PatrolPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
        UAnimMontage* Montage;

    class UWidgetComponent* WidgetComp;

    float const MaxHealth = 582.0f;

    float Health;

    int32 Exp;

    /*
    UFUNCTION()
        void SeePawn(APawn* Pawn);
    */

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
        class UBoxComponent* RightFistCollisionBox;

};
