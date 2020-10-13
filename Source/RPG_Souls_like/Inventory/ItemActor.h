// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "ItemActor.generated.h"

// item type
enum EItemType {
	E_Weapon,
	E_Consumable,
	E_Task
};

//item info table
USTRUCT(BlueprintType)
struct FTableInformation : public FTableRowBase {
	GENERATED_BODY()
	
	FTableInformation() { ; }

	//item name
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	//item level
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	uint8 UseGrade;

	//item texture
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UTexture2D* Texture;

	//item intro
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString Intro;
	
};

//item info
struct FItemInformation {
	//item name
	FString ItemName;

	//item level
	uint8 ItemUseGrade;

	//item texture
	class UTexture2D* ItemTexture;

	//item intro
	FString ItemIntro;
};

struct FItemAllType {
	//item type
	TEnumAsByte<EItemType> ItemType;

	bool CanUse;

	bool CanOverlap;
};

UCLASS()
class RPG_SOULS_LIKE_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* MeshComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UDataTable* DataTable;

	FItemInformation ItemInformation;

	//load data
	void LoadItemInfo(FName Name);

	// skeletal mesh component
	

	//collision component
	class UCapsuleComponent* CapsuleComponent;

	//root component
	class USceneComponent* SceneComponent;

	//collision event
	UFUNCTION()
	void OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool ItemPickUpEvent(AActor* OtherActor);

	//if the slot is empty
	bool ItemIsEmpty(uint16& Index, class UItemUserWidget* ItemBag);

	//detect if the bag has the same item
	bool ItemIsTheSame(uint16& Index, class UItemUserWidget* ItemBag);

	uint8 PickUpItemAmount = 1;

//public:	
	// Called every frame
//	virtual void Tick(float DeltaTime) override;

public:
	FItemAllType ItemAllType;

	//detect item type
	void DetectItemType(class UItemUserWidget*& ItemUserWidget);

	virtual bool UseItem();

	void ReduceUsedBagSpace();

	//weapon base
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AWeaponItemActor> WeaponClass;

	class AWeaponItemActor* Weapon;

	class ARPG_Souls_likeCharacter* BaseCharacter;

	bool Pickable;
};