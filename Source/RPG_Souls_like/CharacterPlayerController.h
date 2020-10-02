// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CharacterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SOULS_LIKE_API ACharacterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

    ACharacterPlayerController();

    virtual void BeginPlay() override;
    
    //bag
    class UBagUserWidget* BagUserWidget;

    //character attribute UI
    class UCharacterAttributeUserWidget* AttributeUserWidget;

private:

    virtual void SetupInputComponent() override;

    //switch movement state
    void CharacterMoveTypeChange();

    //open bag
    void OpenBagEvent();

    void ShowMouseEvent();

    //open character attribute UI
    UFUNCTION()
        void OpenCharacterAttributeUI();
};
