// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsumableItemActor.h"
#include "RPG_Souls_like/RPG_Souls_likeCharacter.h"
#include "RPG_Souls_like/CharacterPlayerController.h"
#include "Kismet/GameplayStatics.h"

AConsumableItemActor::AConsumableItemActor() 
{
	ItemAllType.ItemType = E_Consumable;
	ItemAllType.CanOverlap = true;
	ItemAllType.CanUse = true;

	//load item info
	LoadItemInfo(TEXT("100"));

	Pc = Cast<ACharacterPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

bool AConsumableItemActor::UseItem()
{
	//SetupHealth();

	return true;
}

void AConsumableItemActor::SetupHealth()
{
	
	if (Pc) {
		UE_LOG(LogTemp, Warning, TEXT("controller"));
		ARPG_Souls_likeCharacter* Player = Cast<ARPG_Souls_likeCharacter>(Pc->GetPawn());

		if (Player) {
			UE_LOG(LogTemp, Warning, TEXT("Used potion"));
			int32 const NewHealth = Player->GetCharacterProperty().CharacterCurrentHp + 100;
			Player->SetHealth(NewHealth);
		}
	}
}
