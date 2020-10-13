// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "RPG_Souls_likeCharacter.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SOULS_LIKE_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
    AAICharacter();

    virtual void BeginPlay() override;

    class USkeletalMesh* SkeletalMesh;

    class UPawnSensingComponent* SensingComponent;

    UPROPERTY(EditAnywhere, Category = AI)
        class UBehaviorTree* BehaviorTree;

private:
    /*
    UFUNCTION()
        void SeePawn(APawn* Pawn);
    */
};
