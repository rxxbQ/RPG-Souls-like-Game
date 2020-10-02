// Fill out your copyright notice in the Description page of Project Settings.


#include "BagUserWidget.h"
#include "Components/Button.h"
#include "Engine/Texture2D.h"
#include "Sound/SoundWave.h"
#include "Kismet/GameplayStatics.h"
#include "CharacterPlayerController.h"
#include "ItemUserWidget.h"

UBagUserWidget::UBagUserWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{

}

bool UBagUserWidget::Initialize()
{
	if (!Super::Initialize()) {
		return false;
	}

	WeaponButton = Cast<UButton>(GetWidgetFromName("WeaponButton"));
	WeaponButton->OnClicked.AddDynamic(this, &UBagUserWidget::WeaponButtonEvent);
	WeaponButton->OnClicked.AddDynamic(this, &UBagUserWidget::OnHoverMusicEvent);

	ConsumableButton = Cast<UButton>(GetWidgetFromName("ConsumableButton"));
	ConsumableButton->OnClicked.AddDynamic(this, &UBagUserWidget::ConsumableButtonEvent);
	ConsumableButton->OnClicked.AddDynamic(this, &UBagUserWidget::OnHoverMusicEvent);

	TaskButton = Cast<UButton>(GetWidgetFromName("TaskButton"));
	TaskButton->OnClicked.AddDynamic(this, &UBagUserWidget::TaskButtonEvent);
	TaskButton->OnClicked.AddDynamic(this, &UBagUserWidget::OnHoverMusicEvent);

	CloseBagButton = Cast<UButton>(GetWidgetFromName("CloseBagButton"));
	CloseBagButton->OnClicked.AddDynamic(this, &UBagUserWidget::CloseButtonEvent);
	CloseBagButton->OnClicked.AddDynamic(this, &UBagUserWidget::ConsumableButtonEvent);

	//UTexture2D* ButtonNormal = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Assets/UI/BarsBack.BarsBack'"));

	//WeaponButton->WidgetStyle.Normal.SetResourceObject(ButtonNormal);
	//WeaponButton->WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Image);
	//ConsumableButton->WidgetStyle.Normal.SetResourceObject(ButtonNormal);
	//ConsumableButton->WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Image);
	//TaskButton->WidgetStyle.Normal.SetResourceObject(ButtonNormal);
	//TaskButton->WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Image);

	/* initialize */
	WeaponItemUserWidget = Cast<UItemUserWidget>(GetWidgetFromName("WeaponItemUserWidget"));
	WeaponItemUserWidget->SetVisibility(ESlateVisibility::Visible);

	ConsumableItemUserWidget = Cast<UItemUserWidget>(GetWidgetFromName("ConsumableItemUserWidget"));
	ConsumableItemUserWidget->SetVisibility(ESlateVisibility::Hidden);

	TaskItemUserWidget = Cast<UItemUserWidget>(GetWidgetFromName("TaskItemUserWidget"));
	TaskItemUserWidget->SetVisibility(ESlateVisibility::Hidden);

	return true;
	
}

void UBagUserWidget::OnHoverMusicEvent()
{
	USoundWave* Music = LoadObject<USoundWave>(NULL, TEXT("SoundWave'/Game/Assets/Music/ui_button_confirm.ui_button_confirm'"));
	UGameplayStatics::PlaySound2D(this, Music, 1.0f);
}

void UBagUserWidget::OnClickedMusicEvent()
{
	USoundWave* Music = LoadObject<USoundWave>(NULL, TEXT("SoundWave'/Game/Assets/Music/button.button'"));
	UGameplayStatics::PlaySound2D(this, Music, 2.0f);
}

void UBagUserWidget::WeaponButtonEvent()
{
	OnClickedMusicEvent();
	WeaponItemUserWidget->SetVisibility(ESlateVisibility::Visible);
	ConsumableItemUserWidget->SetVisibility(ESlateVisibility::Hidden);
	TaskItemUserWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UBagUserWidget::ConsumableButtonEvent()
{
	OnClickedMusicEvent();
	WeaponItemUserWidget->SetVisibility(ESlateVisibility::Hidden);
	ConsumableItemUserWidget->SetVisibility(ESlateVisibility::Visible);
	TaskItemUserWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UBagUserWidget::TaskButtonEvent()
{
	OnClickedMusicEvent();
	WeaponItemUserWidget->SetVisibility(ESlateVisibility::Hidden);
	ConsumableItemUserWidget->SetVisibility(ESlateVisibility::Hidden);
	TaskItemUserWidget->SetVisibility(ESlateVisibility::Visible);
}

void UBagUserWidget::CloseButtonEvent()
{
	OnClickedMusicEvent();

	ACharacterPlayerController* Pc = Cast<ACharacterPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (Pc) {
		Pc->BagUserWidget->RemoveFromParent();
	}
}
