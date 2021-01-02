// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponItemActor.h"
#include "RPG_Souls_like/RPG_Souls_likeCharacter.h"
#include "Kismet/GameplayStatics.h"

AWeaponItemActor::AWeaponItemActor() 
{
	ItemAllType.CanOverlap = false;
	ItemAllType.CanUse = false;

	DefaultWeaponName = FName("");

	WeaponDataTable = LoadObject<UDataTable>(NULL, TEXT("DataTable'/Game/Assets/DT_WeaponData.DT_WeaponData'"));

	//load item info
	//if (DefaultWeaponName != "") {
	//	LoadItemInfo(DefaultWeaponName);
	//}	
}

bool AWeaponItemActor::UseItem()
{
	return true;
}

void AWeaponItemActor::SetupWeapon(FName WeaponName)
{
	if (WeaponDataTable) {
		//static const FString PString = FString("BlackKnightDT");
		DefaultWeaponName = WeaponName;
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(WeaponName, TEXT("Name"), true);
		if (WeaponData) {
			MeshComponent->SetSkeletalMesh(WeaponData->WeaponMesh);
			ItemInformation.ItemTexture = WeaponData->WeaponTexture;

			Damage = WeaponData->Damage;
			StrengthBuff = WeaponData->StrengthBuff;
			AgilityBuff = WeaponData->AgilityBuff;
			/*
			switch (WeaponData->WeaponType.GetValue()) {
				case E_Attack:

					break;
				case E_Defence:

					break;
			}
			*/
		}
	}	
}

void AWeaponItemActor::BeginPlay()
{
	Super::BeginPlay();

	if (DefaultWeaponName != "") {
		LoadItemInfo(DefaultWeaponName);
		SetupWeapon(DefaultWeaponName);
	}

	//WeaponCollisionBox->OnComponentHit.AddDynamic(this, &ARPG_Souls_likeCharacter::OnAttackHit);
}
