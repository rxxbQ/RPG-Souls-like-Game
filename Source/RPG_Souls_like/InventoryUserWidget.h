// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/ItemActor.h"
#include "InventoryUserWidget.generated.h"


struct FInventoryInfo {
	//item class
	TSubclassOf<class AItemActor> ItemClass;

	//item amount
	uint8 ItemAmount = 0;

	//if item is empty
	bool ItemClassIsEmpty() {
		return ItemClass == NULL? true:false ;
	}

	//if the item is full, item amount
	void SetItemAmount(uint8 NewAmount) {
		ItemAmount = NewAmount;
	}
};
/**
 * 
 */
UCLASS()
class RPG_SOULS_LIKE_API UInventoryUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UInventoryUserWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	class UBorder* ItemBorder;

	class UImage* ItemImage;

	class UTextBlock* ItemAmount;

	FInventoryInfo InventoryInfo;

	//change inventory item info
	void ChangeInventoryItemInfo(TSubclassOf<class AItemActor> LocalItemClass, class UTexture2D* LocalTexture, uint8 LocalItemAmount);

	//display item amount
	void ShowItemAmount();

	//item amount text
	void AddItemAmountText(uint8 LocalAmount);

	class ARPG_Souls_likeCharacter* BaseCharacter;
private:

	//mouse down event
	UFUNCTION()
		FEventReply MouseDownEvent(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

	//mouse up event
	UFUNCTION()
		FEventReply MouseUpEvent(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

	//get animation from editor
	void GetWidgetAnimation(TMap<FString, UWidgetAnimation*>& OutAnimation);

	//animation map
	TMap<FString, UWidgetAnimation*> WidgetAnimationMap;

	//clear slot
	void ClearInventory();

};
