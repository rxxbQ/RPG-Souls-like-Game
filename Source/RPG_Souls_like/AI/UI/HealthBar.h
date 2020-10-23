// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SOULS_LIKE_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetBarValuePercent(float const Value);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UProgressBar* HealthValue = nullptr;
	
};
