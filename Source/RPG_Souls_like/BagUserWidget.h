// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BagUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SOULS_LIKE_API UBagUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UBagUserWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	class UItemUserWidget* WeaponItemUserWidget;

	class UItemUserWidget* ConsumableItemUserWidget;

	class UItemUserWidget* TaskItemUserWidget;

private:

	class UButton* WeaponButton;
	class UButton* ConsumableButton;
	class UButton* TaskButton;
	class UButton* CloseBagButton;

	// hover music
	UFUNCTION()
		void OnHoverMusicEvent();

	//click music
	UFUNCTION()
		void OnClickedMusicEvent();

	UFUNCTION()
		void WeaponButtonEvent();

	UFUNCTION()
		void ConsumableButtonEvent();

	UFUNCTION()
		void TaskButtonEvent();

	UFUNCTION()
		void CloseButtonEvent();
};
