// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "Engine/DataTable.h"
#include "Components/BoxComponent.h"
#include "WeaponItemActor.generated.h"

//weapon type
UENUM (BlueprintType)
enum EWeaponType {
	E_Attack,
	E_Defence
};

USTRUCT (BlueprintType)
struct FWeaponData : public FTableRowBase 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		class USkeletalMesh* WeaponMesh;

	UPROPERTY(EditAnywhere)
		FString WeaponName;

	UPROPERTY(EditAnywhere)
		class UTexture2D* WeaponTexture;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<EWeaponType> WeaponType;

	UPROPERTY(EditAnywhere)
		float Damage;

	UPROPERTY(EditAnywhere)
		float StrengthBuff;

	UPROPERTY(EditAnywhere)
		float AgilityBuff;

	UPROPERTY(EditAnywhere)
		FVector BoxExtent;
};

/**
 * 
 */
UCLASS()
class RPG_SOULS_LIKE_API AWeaponItemActor : public AItemActor
{
	GENERATED_BODY()
	
public:
	AWeaponItemActor();
	
	virtual bool UseItem();

	virtual void SetupWeapon(FName WeaponName);

	float Damage;

	float StrengthBuff;

	float AgilityBuff;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		class UDataTable* WeaponDataTable;

	FWeaponData* WeaponData;

	UPROPERTY(EditAnywhere)
		FName DefaultWeaponName;
};
