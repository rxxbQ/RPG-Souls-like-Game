// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponItemActor.h"
#include "RPG_Souls_like/RPG_Souls_likeCharacter.h"

#include "Kismet/GameplayStatics.h"

AWeaponItemActor::AWeaponItemActor() 
{
	ItemAllType.ItemType = E_Weapon;
	ItemAllType.CanOverlap = false;
	ItemAllType.CanUse = false;

	DefaultWeaponName = FName("");

	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionBox"));
	WeaponCollisionBox->SetupAttachment(RootComponent);
	WeaponCollisionBox->SetHiddenInGame(false);
	WeaponCollisionBox->SetCollisionProfileName("NoCollision");
	WeaponCollisionBox->SetNotifyRigidBodyCollision(false);

	ShieldCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldCollisionBox"));
	ShieldCollisionBox->SetupAttachment(RootComponent);
	ShieldCollisionBox->SetHiddenInGame(false);
	ShieldCollisionBox->SetCollisionProfileName("NoCollision");
	ShieldCollisionBox->SetNotifyRigidBodyCollision(false);

	//load item info
	//if (DefaultWeaponName != "") {
	//	LoadItemInfo(DefaultWeaponName);
	//}	
}

bool AWeaponItemActor::UseItem()
{
	//SetupWeapon(DefaultWeaponName);
	//AItemActor::UseItem();
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *DefaultWeaponName.ToString());
	//WeaponOnHand = DefaultWeaponName;
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *WeaponOnHand.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("this is%s"), *ItemInformation.ItemName);
	//UE_LOG(LogTemp, Warning, TEXT("Used weapon"));
	if (DefaultWeaponName == FName("BlackKnight")) {
		WeaponNum = 1;
	}
	else if (DefaultWeaponName == FName("DragonSword")) {
		WeaponNum = 3;
	}

	return true;
}

void AWeaponItemActor::SetupWeapon(FName WeaponName)
{
	if (WeaponDataTable) {
		//static const FString PString = FString("BlackKnightDT");
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(WeaponName, TEXT("Name"), true);
		if (WeaponData) {
			MeshComponent->SetSkeletalMesh(WeaponData->WeaponMesh);
			ItemInformation.ItemTexture = WeaponData->WeaponTexture;

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
