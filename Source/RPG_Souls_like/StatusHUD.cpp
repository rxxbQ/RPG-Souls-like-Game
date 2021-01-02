// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusHUD.h"
#include "RPG_Souls_like/RPG_Souls_likeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

UStatusHUD::UStatusHUD(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{

}

bool UStatusHUD::Initialize()
{
	if (!Super::Initialize()) {
		return false;
	}
	
	//get character object
	BaseCharacter = Cast<ARPG_Souls_likeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!BaseCharacter) {
		UE_LOG(LogTemp, Warning, TEXT("StatusHUD.cpp::Initialize()::Ch == NULL"));
	}

	//character level text block
	CharacterLevel = Cast<UTextBlock>(GetWidgetFromName("LevelText"));
	if (CharacterLevel) {
		CharacterLevel->TextDelegate.BindUFunction(this, "GetLevel");
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("StatusHUD.cpp::Initialize()::CharacterLevel == NULL"));
	}

	return true;
}

void UStatusHUD::SetHealthValuePercent(float const Value)
{
	HealthValue->SetPercent(Value);
}

void UStatusHUD::SetManaValuePercent(float const Value)
{
	ManaValue->SetPercent(Value);
}

void UStatusHUD::SetStaminaValuePercent(float const Value)
{
	StaminaValue->SetPercent(Value);
}

FText UStatusHUD::GetLevel()
{
	FString NewString = FString::FromInt(BaseCharacter->GetCharacterProperty().CharacterLevel);
	return FText::FromString(NewString);
}
