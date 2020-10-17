// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "ChangeSpeed.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SOULS_LIKE_API UChangeSpeed : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
    UChangeSpeed();

    void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
    FString GetStaticServiceDescription() const;

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
        float Speed = 600.0f;

};
