// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "Engine/DataTable.h"
#include "Components/BoxComponent.h"
#include "WeaponItemActor.generated.h"

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

	void SetupWeapon(FName WeaponName);

	FName WeaponOnHand;

	//weapon number
	uint8 WeaponNum;

	//collision box
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* WeaponCollisionBox;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		class UDataTable* WeaponDataTable;

	FWeaponData* WeaponData;

	UPROPERTY(EditAnywhere)
		FName DefaultWeaponName;
};
