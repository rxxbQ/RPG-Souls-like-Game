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

    UAnimMontage* GetAttackMontage() const;

    float GetHealth() const;

    float GetMaxHealth() const;

    int GetExp() const;

    void SetHealth(float const NewHealth);

    virtual void AttackStart();

    virtual void AttackEnd();

    UFUNCTION()
        bool CanBeAssassinated();

    void ExecuteStealth();

    class AWeaponItemActor* Weapon;

    class AConsumableItemActor* Consumable;

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

    FTimerHandle DeathTimeHandler;

    void HitReaction();

    void ToggleLockOnTargetWidget(bool IsTargeted);

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
        APatrolPath* PatrolPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
        UAnimMontage* AttackMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
       class UAnimMontage* ExecutionMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
        class UAnimMontage* HitReactionMontage;

    class UWidgetComponent* HealthWidgetComp;

    class UWidgetComponent* LockOnTargetWidget;

    float const MaxHealth = 582.0f;

    float Health;

    int32 Exp;

    // weapon base
    UPROPERTY(EditAnywhere)
        TSubclassOf<class AWeaponItemActor> WeaponClass;

    // item base
    UPROPERTY(EditAnywhere)
        TSubclassOf<class AConsumableItemActor> ConsumableClass;

    //spawn weapon
    void SpawnWeapon();

    //spawn loot after death
    void SpawnLoot();

    TArray<AActor*> AlreadyDamagedPlayer;

    void ExecuteRadgoll();

    void Death();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbleToAttack", meta = (AllowPrivateAccess = "true"))
        bool AbleToAttack;

    /*
    UFUNCTION()
        void SeePawn(APawn* Pawn);
    */

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
        class UBoxComponent* RightFistCollisionBox;

};
