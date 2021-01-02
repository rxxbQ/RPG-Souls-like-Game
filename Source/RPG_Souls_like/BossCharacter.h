// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossCharacter.generated.h"

UCLASS()
class RPG_SOULS_LIKE_API ABossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditAnywhere, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	void ShowHealthBar();

	uint32 GetExp() const;

	float GetHealth() const;

	float GetAttackDefence() const;

	float GetMaxHealth() const;

	void SetHealth(float const NewHealth);

	void AttackStart();

	void AttackEnd();

	// set super armor to true
	void SuperArmorStart();

	// set super armor to false
	void SuperArmorEnd();

	void MeleeAttack();

	void HitReaction();

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

	void ToggleLockOnTargetWidget(bool IsTargeted);

	FTimerHandle DeathTimeHandler;

	UAnimMontage* GetAttackMontage() const;

	bool BattleStart;

private:
	float const MaxHealth = 1059;

	float Health;

	uint32 Exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbleToAttack", meta = (AllowPrivateAccess = "true"))
		bool AbleToAttack;

	class UWidgetComponent* LockOnTargetWidget;

	UPROPERTY(EditAnywhere, Category = "UI HUD")
		TSubclassOf<UUserWidget> BossHealthWidgetClass;

	UUserWidget* BossHealthWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AttackComboMontage_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AttackComboMontage_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AttackComboMontage_3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* ReactMontage;

	//second stage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* BattlecryMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* DeathMontage;

	UAnimMontage* AttackMontage;

	//weapon base
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABladeWeapon> WeaponClass;

	class ABladeWeapon* Weapon;

	// spawn weapon
	void SpawnWeapon();

	void Death();

	TArray<AActor*> AlreadyDamagedPlayer;

	// no hit reaction
	bool SuperArmor;

	bool SecondStage;

	float AttackDefence;

	// if decreases to 0, super armor disappears
	uint8 Endurance;
};
