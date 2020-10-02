// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "InventoryUserWidget.h"
#include "ItemUserWidget.generated.h"


struct FItemBagInfo {
	class UUniformGridSlot* GridSlot;

	class UInventoryUserWidget* InventoryUserWidget;

	//slot is empty
	bool InventoryItemClassIsEmpty(){
		return InventoryUserWidget->InventoryInfo.ItemClassIsEmpty();
	}

	//if item is the same
	bool ItemClassIsTheSame(TSubclassOf<class AItemActor> LocalItemClass) {
		if (!InventoryUserWidget->InventoryInfo.ItemClassIsEmpty()) {
			return LocalItemClass == InventoryUserWidget->InventoryInfo.ItemClass ? true : false;
		}
		
		return false;
	}

	//if the slot is full
	bool ItemSlotIsFull() {
		return InventoryUserWidget->InventoryInfo.ItemAmount <= 99 ? true: false;
	}

	//if the slot is full after picking up the item
	bool ItemAmountPickUpIsFull(uint8 LocalAmount, uint8& ItemLeft ){

		if (InventoryUserWidget->InventoryInfo.ItemAmount + LocalAmount > 99) {
			//slot is full
			InventoryUserWidget->InventoryInfo.SetItemAmount(99);
			//extra item amount
			ItemLeft = InventoryUserWidget->InventoryInfo.ItemAmount + LocalAmount - 99;

			return true;
		}
		return false;
	}

	//pick up overlap item type
	void PickUpCanOverlapItem(uint8 LocalAmount) {
		InventoryUserWidget->InventoryInfo.ItemAmount += LocalAmount;
		//set item amount text
		InventoryUserWidget->AddItemAmountText(InventoryUserWidget->InventoryInfo.ItemAmount);
	}

};

/**
 * 
 */
UCLASS()
class RPG_SOULS_LIKE_API UItemUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UItemUserWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	class UUniformGridPanel* BagUniformGridPanel;

	TArray<FItemBagInfo> ItemBagInfo;

	//row
	uint8 ItemRow = 0;

	//column
	uint8 ItemColumn = 0;

	uint16 ArrayIndex = 0;

	//bag capacity
	void SetItemCapacity(uint8 AddAmount);

	//bag used
	void SetItemCurrentAmount(uint8 AddNewAmount);

	//reduce used amount
	void ReduceItemCurrentAmount(uint8 NewAmount);

private:
	uint16 ItemCapacity;

	uint16 ItemCurrentAmount = 0;

	class UTextBlock* ItemAmountText;
};
