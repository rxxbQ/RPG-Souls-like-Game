// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttributeUserWidget.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "RPG_Souls_like/CharacterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "RPG_Souls_like/RPG_Souls_likeCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

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


	/* character attribute text block*/
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
		CharacterLevel->TextDelegate.BindUFunction(this, "GetLevel");
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::CharacterLevel == NULL"));
	}

	//character max exp text block
	CharacterMaxExp = Cast<UTextBlock>(GetWidgetFromName("CharacterMaxExp"));
	if (CharacterMaxExp) {
		CharacterMaxExp->TextDelegate.BindUFunction(this, "GetMaxExp");
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::CharacterMaxExp == NULL"));
	}

	//character current exp text block
	CharacterCurrentExp = Cast<UTextBlock>(GetWidgetFromName("CharacterCurrentExp"));
	if (CharacterCurrentExp) {
		CharacterCurrentExp->TextDelegate.BindUFunction(this, "GetCurrentExp");
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::CharacterCurrentExp == NULL"));
	}

	//character max hp text block
	CharacterMaxHp = Cast<UTextBlock>(GetWidgetFromName("CharacterMaxHp"));
	if (CharacterMaxHp) {
		CharacterMaxHp->TextDelegate.BindUFunction(this, "GetHealth");
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::CharacterMaxHp == NULL"));
	}

	//character max mp text block
	CharacterMaxMp = Cast<UTextBlock>(GetWidgetFromName("CharacterMaxMp"));
	if (CharacterMaxMp) {
		CharacterMaxMp->TextDelegate.BindUFunction(this, "GetMana");
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::CharacterMaxMp == NULL"));
	}

	//character max stamina text block
	CharacterMaxStamina = Cast<UTextBlock>(GetWidgetFromName("CharacterMaxStamina"));
	if (CharacterMaxStamina) {
		CharacterMaxStamina->TextDelegate.BindUFunction(this, "GetStamina");
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::CharacterMaxStamina == NULL"));
	}

	//character vitality text block
	CharacterVitality = Cast<UTextBlock>(GetWidgetFromName("CharacterVitality"));
	if (CharacterVitality) {
		CharacterVitality->TextDelegate.BindUFunction(this, "GetVitality");
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::CharacterVitality == NULL"));
	}

	//character attunement text block
	CharacterAttunement = Cast<UTextBlock>(GetWidgetFromName("CharacterAttunement"));
	if (CharacterAttunement) {
		CharacterAttunement->TextDelegate.BindUFunction(this, "GetAttunement");
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::CharacterAttunement == NULL"));
	}

	//character strength text block
	CharacterStrength = Cast<UTextBlock>(GetWidgetFromName("CharacterStrength"));
	if (CharacterStrength) {
		CharacterStrength->TextDelegate.BindUFunction(this, "GetStrength");
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::CharacterStrength == NULL"));
	}

	//character agility text block
	CharacterAgility = Cast<UTextBlock>(GetWidgetFromName("CharacterAgility"));
	if (CharacterAgility) {
		CharacterAgility->TextDelegate.BindUFunction(this, "GetAgility");
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::CharacterAgility == NULL"));
	}

	//character intelligence text block
	CharacterIntelligence = Cast<UTextBlock>(GetWidgetFromName("CharacterIntelligence"));
	if (CharacterIntelligence) {
		CharacterIntelligence->TextDelegate.BindUFunction(this, "GetIntelligence");
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::CharacterIntelligence == NULL"));
	}

	//character endurance
	CharacterEndurance = Cast<UTextBlock>(GetWidgetFromName("CharacterEndurance"));
	if (CharacterEndurance) {
		CharacterEndurance->TextDelegate.BindUFunction(this, "GetEndurance");
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::CharacterEndurance == NULL"));
	}

	//character resistance
	CharacterResistance = Cast<UTextBlock>(GetWidgetFromName("CharacterResistance"));
	if (CharacterResistance) {
		CharacterResistance->TextDelegate.BindUFunction(this, "GetResistance");
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::CharacterResistance == NULL"));
	}

	//character movementspeed
	CharacterMovementSpeed = Cast<UTextBlock>(GetWidgetFromName("CharacterMovementSpeed"));
	if (CharacterMovementSpeed) {
		CharacterMovementSpeed->TextDelegate.BindUFunction(this, "GetMovementSpeed");
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::CharacterMovementSpeed == NULL"));
	}

	//character physics damage
	CharacterPhysics = Cast<UTextBlock>(GetWidgetFromName("CharacterPhysics"));
	if (CharacterPhysics) {
		CharacterPhysics->TextDelegate.BindUFunction(this, "GetPhysics");
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::CharacterPhysics == NULL"));
	}

	//character magic damage
	CharacterMagic = Cast<UTextBlock>(GetWidgetFromName("CharacterMagic"));
	if (CharacterMagic) {
		CharacterMagic->TextDelegate.BindUFunction(this, "GetMagic");
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::CharacterMagic == NULL"));
	}

	/* level up buttons*/
	//initialize VitalityLevelUp button
	VitalityLevelUp = Cast<UButton>(GetWidgetFromName("VitalityLevelUp"));

	if (VitalityLevelUp) {
		VitalityLevelUp->SetVisibility(ESlateVisibility::Hidden);

		VitalityLevelUp->OnClicked.AddDynamic(this, &UCharacterAttributeUserWidget::LevelUpVitality);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::VitalityLevelUp == NULL"));
	}

	//initialize AttunementLevelUp button
	AttunementLevelUp = Cast<UButton>(GetWidgetFromName("AttunementLevelUp"));

	if (AttunementLevelUp) {
		AttunementLevelUp->SetVisibility(ESlateVisibility::Hidden);

		AttunementLevelUp->OnClicked.AddDynamic(this, &UCharacterAttributeUserWidget::LevelUpAttunement);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::AttunementLevelUp == NULL"));
	}

	//initialize StrengthLevelUp button
	StrengthLevelUp = Cast<UButton>(GetWidgetFromName("StrengthLevelUp"));

	if (StrengthLevelUp) {
		StrengthLevelUp->SetVisibility(ESlateVisibility::Hidden);

		StrengthLevelUp->OnClicked.AddDynamic(this, &UCharacterAttributeUserWidget::LevelUpStrength);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::StrengthLevelUp == NULL"));
	}

	//initialize AgilityLevelUp button
	AgilityLevelUp = Cast<UButton>(GetWidgetFromName("AgilityLevelUp"));

	if (AgilityLevelUp) {
		AgilityLevelUp->SetVisibility(ESlateVisibility::Hidden);

		AgilityLevelUp->OnClicked.AddDynamic(this, &UCharacterAttributeUserWidget::LevelUpAgility);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::AgilityLevelUp == NULL"));
	}

	//initialize IntelligenceLevelUp button
	IntelligenceLevelUp = Cast<UButton>(GetWidgetFromName("IntelligenceLevelUp"));

	if (IntelligenceLevelUp) {
		IntelligenceLevelUp->SetVisibility(ESlateVisibility::Hidden);

		IntelligenceLevelUp->OnClicked.AddDynamic(this, &UCharacterAttributeUserWidget::LevelUpIntelligence);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::IntelligenceLevelUp == NULL"));
	}

	//initialize EnduranceLevelUp button
	EnduranceLevelUp = Cast<UButton>(GetWidgetFromName("EnduranceLevelUp"));

	if (EnduranceLevelUp) {
		EnduranceLevelUp->SetVisibility(ESlateVisibility::Hidden);

		EnduranceLevelUp->OnClicked.AddDynamic(this, &UCharacterAttributeUserWidget::LevelUpEndurance);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::EnduranceLevelUp == NULL"));
	}

	//initialize ResistanceLevelUp button
	ResistanceLevelUp = Cast<UButton>(GetWidgetFromName("ResistanceLevelUp"));

	if (ResistanceLevelUp) {
		ResistanceLevelUp->SetVisibility(ESlateVisibility::Hidden);

		ResistanceLevelUp->OnClicked.AddDynamic(this, &UCharacterAttributeUserWidget::LevelUpResistance);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeUserWidget.cpp::Initialize()::ResistanceLevelUp == NULL"));
	}

	return true;
}

void UCharacterAttributeUserWidget::CloseUIEvent()
{
	ACharacterPlayerController* Pc = Cast<ACharacterPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (Pc) {
		Pc->AttributeUserWidget->RemoveFromParent();
	}
}

//health text
FText UCharacterAttributeUserWidget::GetHealth()
{
	return BaseCharacter->GetCharacterBasic(BaseCharacter->GetCharacterProperty().CharacterMaxHp, BaseCharacter->GetCharacterProperty().CharacterCurrentHp, TEXT("HP : "));
}

//mana text
FText UCharacterAttributeUserWidget::GetMana()
{
	return BaseCharacter->GetCharacterBasic(BaseCharacter->GetCharacterProperty().CharacterMaxMp, BaseCharacter->GetCharacterProperty().CharacterCurrentMp, TEXT("MP : "));
}

//stamina text
FText UCharacterAttributeUserWidget::GetStamina()
{
	return BaseCharacter->GetCharacterBasic(uint32(BaseCharacter->GetCharacterProperty().CharacterMaxStamina), uint32(BaseCharacter->GetCharacterProperty().CharacterCurrentStamina), TEXT("Stamina : "));
}

//level text
FText UCharacterAttributeUserWidget::GetLevel()
{
	FString NewString = TEXT("Level : ") + FString::FromInt(BaseCharacter->GetCharacterProperty().CharacterLevel);
	return FText::FromString(NewString);
}

//vitality text
FText UCharacterAttributeUserWidget::GetVitality()
{
	FString NewString = TEXT("Vitality : ") + FString::FromInt(BaseCharacter->GetCharacterProperty().CharacterVitality);
	return FText::FromString(NewString);
}

//attunement
FText UCharacterAttributeUserWidget::GetAttunement()
{
	FString NewString = TEXT("Attunement : ") + FString::FromInt(BaseCharacter->GetCharacterProperty().CharacterAttunement);
	return FText::FromString(NewString);
}

//strength text
FText UCharacterAttributeUserWidget::GetStrength()
{
	FString NewString = TEXT("Strength : ") + FString::FromInt(BaseCharacter->GetCharacterProperty().CharacterStrength);
	return FText::FromString(NewString);
}

//agility text
FText UCharacterAttributeUserWidget::GetAgility()
{
	FString NewString = TEXT("Agility : ") + FString::FromInt(BaseCharacter->GetCharacterProperty().CharacterAgility);
	return FText::FromString(NewString);
}

//intelligence text
FText UCharacterAttributeUserWidget::GetIntelligence()
{
	FString NewString = TEXT("Intelligence : ") + FString::FromInt(BaseCharacter->GetCharacterProperty().CharacterIntelligence);
	return FText::FromString(NewString);
}

//endurance text
FText UCharacterAttributeUserWidget::GetEndurance()
{
	FString NewString = TEXT("Endurance : ") + FString::FromInt(BaseCharacter->GetCharacterProperty().CharacterEndurance);
	return FText::FromString(NewString);
}

//resistance text
FText UCharacterAttributeUserWidget::GetResistance()
{
	return FText::FromString(TEXT("Resistance : ") +FString::SanitizeFloat(BaseCharacter->GetCharacterProperty().CharacterResistance));
}

//movement speed text
FText UCharacterAttributeUserWidget::GetMovementSpeed()
{
	return FText::FromString(TEXT("MovementSpeed : ") + FString::SanitizeFloat(BaseCharacter->GetCharacterMovement()->MaxWalkSpeed));
}

//required exp text
FText UCharacterAttributeUserWidget::GetMaxExp()
{
	FString NewString = TEXT("Required EXP : ") + FString::FromInt(BaseCharacter->GetCharacterProperty().CharacterMaxExp);
	return FText::FromString(NewString);
}

//current exp text
FText UCharacterAttributeUserWidget::GetCurrentExp()
{
	FString NewString = TEXT("EXP : ") + FString::FromInt(BaseCharacter->GetCharacterProperty().CharacterCurrentExp);
	return FText::FromString(NewString);
}

//physics damage text
FText UCharacterAttributeUserWidget::GetPhysics()
{
	return BaseCharacter->GetCharacterAttribute(BaseCharacter->GetCharacterProperty().CharacterAttackDamage, TEXT("Physics : "));
}

//magic damage text
FText UCharacterAttributeUserWidget::GetMagic()
{
	return BaseCharacter->GetCharacterAttribute(BaseCharacter->GetCharacterProperty().CharacterMagicDamage, TEXT("Magic : "));
}

void UCharacterAttributeUserWidget::LevelUpVitality()
{
	uint32 NewVitality = BaseCharacter->GetCharacterProperty().CharacterVitality + 1;
	BaseCharacter->SetVitality(NewVitality);

	if (BaseCharacter->GetCharacterProperty().SkillPoint > 0) {
		uint8 NewSkillPoint = BaseCharacter->GetCharacterProperty().SkillPoint - 1;
		BaseCharacter->SetSkillPoint(NewSkillPoint);
	}

	HideButton();
}

void UCharacterAttributeUserWidget::LevelUpAttunement()
{
	uint32 NewAttunement = BaseCharacter->GetCharacterProperty().CharacterAttunement + 1;
	BaseCharacter->SetAttunement(NewAttunement);

	if (BaseCharacter->GetCharacterProperty().SkillPoint > 0) {
		uint8 NewSkillPoint = BaseCharacter->GetCharacterProperty().SkillPoint - 1;
		BaseCharacter->SetSkillPoint(NewSkillPoint);
	}

	HideButton();
}

void UCharacterAttributeUserWidget::LevelUpStrength()
{
	uint32 NewStrength = BaseCharacter->GetCharacterProperty().CharacterStrength + 1;
	BaseCharacter->SetStrength(NewStrength);

	if (BaseCharacter->GetCharacterProperty().SkillPoint > 0) {
		uint8 NewSkillPoint = BaseCharacter->GetCharacterProperty().SkillPoint - 1;
		BaseCharacter->SetSkillPoint(NewSkillPoint);
	}

	HideButton();
}

void UCharacterAttributeUserWidget::LevelUpAgility()
{
	uint32 NewAgility = BaseCharacter->GetCharacterProperty().CharacterAgility + 1;
	BaseCharacter->SetAgility(NewAgility);

	if (BaseCharacter->GetCharacterProperty().SkillPoint > 0) {
		uint8 NewSkillPoint = BaseCharacter->GetCharacterProperty().SkillPoint - 1;
		BaseCharacter->SetSkillPoint(NewSkillPoint);
	}

	HideButton();
}

void UCharacterAttributeUserWidget::LevelUpIntelligence()
{
	uint32 NewIntelligence = BaseCharacter->GetCharacterProperty().CharacterIntelligence + 1;
	BaseCharacter->SetIntelligence(NewIntelligence);

	if (BaseCharacter->GetCharacterProperty().SkillPoint > 0) {
		uint8 NewSkillPoint = BaseCharacter->GetCharacterProperty().SkillPoint - 1;
		BaseCharacter->SetSkillPoint(NewSkillPoint);
	}

	HideButton();
}

void UCharacterAttributeUserWidget::LevelUpEndurance()
{
	uint32 NewEndurance = BaseCharacter->GetCharacterProperty().CharacterEndurance + 1;
	BaseCharacter->SetEndurance(NewEndurance);

	if (BaseCharacter->GetCharacterProperty().SkillPoint > 0) {
		uint8 NewSkillPoint = BaseCharacter->GetCharacterProperty().SkillPoint - 1;
		BaseCharacter->SetSkillPoint(NewSkillPoint);
	}

	HideButton();
}

void UCharacterAttributeUserWidget::LevelUpResistance()
{
	float NewResistance = BaseCharacter->GetCharacterProperty().CharacterResistance + 0.05f;
	BaseCharacter->SetResistance(NewResistance);

	if (BaseCharacter->GetCharacterProperty().SkillPoint > 0) {
		uint8 NewSkillPoint = BaseCharacter->GetCharacterProperty().SkillPoint - 1;
		BaseCharacter->SetSkillPoint(NewSkillPoint);
	}
	
	HideButton();
}

void UCharacterAttributeUserWidget::HideButton()
{
	if (BaseCharacter->GetCharacterProperty().SkillPoint <= 0) {
		VitalityLevelUp->SetVisibility(ESlateVisibility::Hidden);
		AttunementLevelUp->SetVisibility(ESlateVisibility::Hidden);
		StrengthLevelUp->SetVisibility(ESlateVisibility::Hidden);
		AgilityLevelUp->SetVisibility(ESlateVisibility::Hidden);
		IntelligenceLevelUp->SetVisibility(ESlateVisibility::Hidden);
		EnduranceLevelUp->SetVisibility(ESlateVisibility::Hidden);
		ResistanceLevelUp->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCharacterAttributeUserWidget::DisplayButton()
{
	VitalityLevelUp->SetVisibility(ESlateVisibility::Visible);
	AttunementLevelUp->SetVisibility(ESlateVisibility::Visible);
	StrengthLevelUp->SetVisibility(ESlateVisibility::Visible);
	AgilityLevelUp->SetVisibility(ESlateVisibility::Visible);
	IntelligenceLevelUp->SetVisibility(ESlateVisibility::Visible);
	EnduranceLevelUp->SetVisibility(ESlateVisibility::Visible);
	ResistanceLevelUp->SetVisibility(ESlateVisibility::Visible);
}
