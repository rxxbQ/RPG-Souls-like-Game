// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryUserWidget.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "Inventory/ItemActor.h"
#include "Kismet/GameplayStatics.h"
#include "ItemUserWidget.h"
#include "CharacterPlayerController.h"
#include "BagUserWidget.h"

UInventoryUserWidget::UInventoryUserWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer) 
{

}

bool UInventoryUserWidget::Initialize()
{
	if (!Super::Initialize()) {
		return false;
	}

	ItemBorder = Cast<UBorder>(GetWidgetFromName("ItemBorder"));

	//bind function
	ItemBorder->OnMouseButtonDownEvent.BindUFunction(this, "MouseDownEvent");
	ItemBorder->OnMouseButtonUpEvent.BindUFunction(this, "MouseUpEvent");

	ItemImage = Cast<UImage>(GetWidgetFromName("ItemImage"));

	ItemAmount = Cast<UTextBlock>(GetWidgetFromName("ItemAmount"));
	ItemAmount->SetVisibility(ESlateVisibility::Collapsed);

	//get animation
	GetWidgetAnimation(WidgetAnimationMap);
	return true;
}

void UInventoryUserWidget::ChangeInventoryItemInfo(TSubclassOf<class AItemActor> LocalItemClass, UTexture2D* LocalTexture, uint8 LocalItemAmount)
{
	InventoryInfo.ItemClass = LocalItemClass;
	InventoryInfo.ItemAmount += LocalItemAmount;

	if (InventoryInfo.ItemAmount > 1) {
		ShowItemAmount();
	}
	else {
		ItemAmount->SetVisibility(ESlateVisibility::Collapsed);
		ItemImage->Brush.SetResourceObject(LocalTexture);
		ItemImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		//ItemImage->Brush.ImageSize();
	}
	
	
}

void UInventoryUserWidget::ShowItemAmount()
{
	if (ItemAmount->GetVisibility() == ESlateVisibility::HitTestInvisible) {

	}
	else {
		ItemAmount->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UInventoryUserWidget::AddItemAmountText(uint8 LocalAmount)
{
	ItemAmount->SetText(FText::AsNumber(LocalAmount));
}

FEventReply UInventoryUserWidget::MouseDownEvent(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.IsMouseButtonDown(TEXT("LeftMouseButton")) && InventoryInfo.ItemClass != NULL) {
		UE_LOG(LogTemp, Warning, TEXT("LMB DOWN"));

		FEventReply Reply;
		Reply.NativeReply = FReply::Handled();
		return Reply;
	}

	if (MouseEvent.IsMouseButtonDown(TEXT("RightMouseButton")) && InventoryInfo.ItemClass != NULL) {
		UE_LOG(LogTemp, Warning, TEXT("RMB DOWN"));

		//play animation
		PlayAnimation(WidgetAnimationMap.FindRef(TEXT("MouseDownAnim")));

		FEventReply Reply;
		Reply.NativeReply = FReply::Unhandled();
		return Reply;
		
	}

	FEventReply Reply;
	Reply.NativeReply = FReply::Unhandled();
	return Reply;
	
}

FEventReply UInventoryUserWidget::MouseUpEvent(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == TEXT("LeftMouseButton") && InventoryInfo.ItemClass != NULL) {
		UE_LOG(LogTemp, Warning, TEXT("LMB UP"));

		FEventReply Reply;
		Reply.NativeReply = FReply::Handled();
		return Reply;
	}

	if (MouseEvent.GetEffectingButton() == TEXT("RightMouseButton") && InventoryInfo.ItemClass != NULL) {
		UE_LOG(LogTemp, Warning, TEXT("RMB UP"));

		class UItemUserWidget* LocalItemUserWidget = NULL;

		//play animation
		PlayAnimation(WidgetAnimationMap.FindRef(TEXT("MouseUpAnim")));

		//get object class
		AItemActor* ItemActor = Cast<AItemActor>(InventoryInfo.ItemClass->GetDefaultObject());
		if (ItemActor) {

			//if item used successfully
			if (ItemActor->UseItem()) {

				//detect item type and reduce current bag space used
				ACharacterPlayerController* Pc = Cast<ACharacterPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

				if (Pc) {
					switch (ItemActor->ItemAllType.ItemType.GetValue()) {
					case E_Weapon:
						UE_LOG(LogTemp, Warning, TEXT("weapon"));
						LocalItemUserWidget = Pc->BagUserWidget->WeaponItemUserWidget;
					
						break;
					case E_Consumable:
						UE_LOG(LogTemp, Warning, TEXT("consumable"));
						LocalItemUserWidget = Pc->BagUserWidget->ConsumableItemUserWidget;
						
						//reduce item amount
						InventoryInfo.ItemAmount -= 1;
						AddItemAmountText(InventoryInfo.ItemAmount);
						break;
					case E_Task:
						UE_LOG(LogTemp, Warning, TEXT("task"));
						LocalItemUserWidget = Pc->BagUserWidget->TaskItemUserWidget;

						//reduce item amount
						InventoryInfo.ItemAmount -= 1;
						AddItemAmountText(InventoryInfo.ItemAmount);
						break;
					}
				}
				

				//if item amount is 0, class is null
				if (InventoryInfo.ItemAmount == 0) {
					LocalItemUserWidget->ReduceItemCurrentAmount(1);
					ClearInventory();
				
				}

				FEventReply Reply;
				Reply.NativeReply = FReply::Handled();
				return Reply;
			}
			else {

				FEventReply Reply;
				Reply.NativeReply = FReply::Unhandled();
				return Reply;
			}
		}

		FEventReply Reply;
		Reply.NativeReply = FReply::Handled();
		return Reply;
	}

	FEventReply Reply;
	Reply.NativeReply = FReply::Unhandled();
	return Reply;
}

void UInventoryUserWidget::GetWidgetAnimation(TMap<FString, UWidgetAnimation*>& OutAnimation)
{
	//clear map
	OutAnimation.Empty();

	FProperty* Property = GetClass()->PropertyLink;

	while (Property != nullptr) {
		if (Property->GetClass() == FObjectProperty::StaticClass()) {
			FObjectProperty* ObjectProperty = Cast<FObjectProperty>(Property);

			if (ObjectProperty->PropertyClass == UWidgetAnimation::StaticClass()) {
				UObject* Object = ObjectProperty->GetObjectPropertyValue_InContainer(this);

				UWidgetAnimation* WidgetAnimation = Cast<UWidgetAnimation>(Object);

				if (WidgetAnimation != nullptr) {
					FString Name = WidgetAnimation->GetMovieScene()->GetName();
					OutAnimation.Add(Name, WidgetAnimation);
				}
			}
		}
		Property = Property->PropertyLinkNext;
	}
}

void UInventoryUserWidget::ClearInventory()
{
	InventoryInfo.ItemClass = NULL;
	InventoryInfo.ItemAmount = 0;

	ItemImage->Brush.SetResourceObject(NULL);
	ItemImage->SetVisibility(ESlateVisibility::Collapsed);
	ItemAmount->SetVisibility(ESlateVisibility::Collapsed);
	AddItemAmountText(0);
}

