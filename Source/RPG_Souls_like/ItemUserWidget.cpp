// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemUserWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "InventoryUserWidget.h"

UItemUserWidget::UItemUserWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	ItemBagInfo.SetNum(500);
}

bool UItemUserWidget::Initialize()
{
	if (!Super::Initialize()) {
		return false;
	}
	
	BagUniformGridPanel = Cast<UUniformGridPanel>(GetWidgetFromName("BagUniformGridPanel"));

	if (BagUniformGridPanel) {
		
		for (ItemColumn, ArrayIndex; ItemColumn < 10; ItemColumn++, ArrayIndex++) {

			//create widget
			ItemBagInfo[ArrayIndex].InventoryUserWidget = CreateWidget<UInventoryUserWidget>(GetGameInstance(), 
				LoadClass<UInventoryUserWidget>(NULL, TEXT("WidgetBlueprint'/Game/UI/InventoryUserWidget.InventoryUserWidget_C'")));

			//add widget
			ItemBagInfo[ArrayIndex].GridSlot = BagUniformGridPanel->AddChildToUniformGrid(ItemBagInfo[ArrayIndex].InventoryUserWidget);

			ItemBagInfo[ArrayIndex].GridSlot->SetRow(ItemRow);
			ItemBagInfo[ArrayIndex].GridSlot->SetColumn(ItemColumn);

			if (ItemColumn == 9) {
				ItemRow++;
				ItemColumn = -1;

				if (ItemRow == 50) {
					break;
				}
			}
		}
		
	}

	ItemCapacity = 10 * (ItemRow);
	ItemAmountText = Cast<UTextBlock>(GetWidgetFromName("ItemAmountText"));
	ItemAmountText->SetText(FText::FromString(TEXT("Capacity: ")+FString::FromInt(ItemCurrentAmount)+TEXT("/")+FString::FromInt(ItemCapacity)));

	return true;
}

void UItemUserWidget::SetItemCapacity(uint8 AddAmount)
{
	ItemCapacity += AddAmount;
	ItemAmountText->SetText(FText::FromString(TEXT("Capacity: ") + FString::FromInt(ItemCurrentAmount) + TEXT("/") + FString::FromInt(ItemCapacity)));
}

void UItemUserWidget::SetItemCurrentAmount(uint8 AddNewAmount)
{
	ItemCurrentAmount += AddNewAmount;
	ItemAmountText->SetText(FText::FromString(TEXT("Capacity: ") + FString::FromInt(ItemCurrentAmount) + TEXT("/") + FString::FromInt(ItemCapacity)));
}

void UItemUserWidget::ReduceItemCurrentAmount(uint8 NewAmount)
{
	ItemCurrentAmount -= NewAmount;
	ItemAmountText->SetText(FText::FromString(TEXT("Capacity: ") + FString::FromInt(ItemCurrentAmount) + TEXT("/") + FString::FromInt(ItemCapacity)));
}
