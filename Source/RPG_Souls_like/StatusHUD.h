// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "StatusHUD.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SOULS_LIKE_API UStatusHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UStatusHUD(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	void SetHealthValuePercent(float const Value);

	void SetManaValuePercent(float const Value);

	void SetStaminaValuePercent(float const Value);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UProgressBar* HealthValue = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UProgressBar* ManaValue = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UProgressBar* StaminaValue = nullptr;

	class UTextBlock* CharacterLevel;

private:
	class ARPG_Souls_likeCharacter* BaseCharacter;

	//level text block
	UFUNCTION()
		FText GetLevel();
};
