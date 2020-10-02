// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttributeUserWidget.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "RPG_Souls_like/CharacterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "RPG_Souls_like/RPG_Souls_likeCharacter.h"

UCharacterAttributeUserWidget::UCharacterAttributeUserWidget(const FObjectInitializer& ObjectInitalizer):Super(ObjectInitalizer) 
{

}

bool UCharacterAttributeUserWidget::Initialize()
{
	if (!Super::Initialize()) {
		return false;
	}
	
	//get character object
	BaseCharacter = Cast<ARPG_Souls_likeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!BaseCharacter) {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::Ch == NULL"));
	}

	//initialize border
	NameBorder = Cast<UBorder>(GetWidgetFromName("NameBorder"));
	
	if (NameBorder) {
		NameBorder->SetRenderOpacity(0.2f);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::NameBorder == NULL"));
	}

	//initialzie name text
	UIName = Cast<UTextBlock>(GetWidgetFromName("UIName"));
	
	if (UIName) {
		UIName->SetText(FText::FromString(TEXT("Status")));

		FLinearColor Color;
		Color.A = 10.0f;
		UIName->SetShadowColorAndOpacity(Color);

		//FSlateFontInfo FontInfo;
		//FontInfo.Size = 35;
		//UIName->SetFont(FontInfo);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::UIName == NULL"));
	}

	//initialize close button
	CloseButton = Cast<UButton>(GetWidgetFromName("CloseButton"));

	if (CloseButton) {

		UObject* Normal = LoadObject<UObject>(NULL, TEXT("Texture2D'/Game/Assets/UI/Close_Normal.Close_Normal'"));
		UObject* Rollover = LoadObject<UObject>(NULL, TEXT("Texture2D'/Game/Assets/UI/Close_Rollover.Close_Rollover'"));

		CloseButton->WidgetStyle.Normal.SetResourceObject(Normal);
		CloseButton->WidgetStyle.Hovered.SetResourceObject(Rollover);

		CloseButton->OnClicked.AddDynamic(this, &UCharacterAttributeUserWidget::CloseUIEvent);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::CloseButton == NULL"));
	}

	//character class text block
	CharacterClass = Cast<UTextBlock>(GetWidgetFromName("CharacterClass"));
	if (CharacterClass) {
		CharacterClass->TextDelegate.BindUFunction(BaseCharacter, "GetCharacterClass");
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::CharacterClass == NULL"));
	}

	//character level text block
	CharacterLevel = Cast<UTextBlock>(GetWidgetFromName("CharacterLevel"));
	if (CharacterLevel) {
		//CharacterLevel->TextDelegate.BindUFunction(, "");
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::CharacterLevel == NULL"));
	}

	CharacterVitality;
	CharacterAttunement;
	CharacterMaxExp;
	CharacterCurrentExp;

	//character max hp text block
	CharacterMaxHp = Cast<UTextBlock>(GetWidgetFromName("CharacterMaxHp"));
	if (CharacterMaxHp) {
		CharacterMaxHp->TextDelegate.BindUFunction(this, "GetHealth");
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::CharacterMaxHp == NULL"));
	}

	CharacterMaxMp;
	CharacterMaxStamina;
	CharacterStrength;
	CharacterAgility;
	CharacterIntelligence;
	CharacterEndurance;
	CharacterResistance;
	CharacterMovementSpeed;

	return true;
}

void UCharacterAttributeUserWidget::CloseUIEvent()
{
	ACharacterPlayerController* Pc = Cast<ACharacterPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (Pc) {
		Pc->AttributeUserWidget->RemoveFromParent();
	}
}

FText UCharacterAttributeUserWidget::GetHealth()
{
	return BaseCharacter->GetCharacterBasic(BaseCharacter->GetCharacterProperty().CharacterMaxHp, BaseCharacter->GetCharacterProperty().CharacterCurrentHp);
}
