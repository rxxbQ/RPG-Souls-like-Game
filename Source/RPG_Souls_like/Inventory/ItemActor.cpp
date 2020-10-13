// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"
#include "Components/CapsuleComponent.h"
#include "RPG_Souls_like/CharacterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "RPG_Souls_like/ItemUserWidget.h"
#include "RPG_Souls_like/BagUserWidget.h"
//#include "WeaponItemActor.h"
//#include "RPG_Souls_like/RPG_Souls_likeCharacter.h"

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DataTable = LoadObject<UDataTable>(NULL, TEXT("DataTable'/Game/Assets/RPGItemInfo.RPGItemInfo'"));

	//initialize root component
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	//initialize skeletal mesh component
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetupAttachment(SceneComponent);

	//initialize collision component
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(SceneComponent);
	CapsuleComponent->SetCapsuleSize(45.0f, 60.0f);

	//setup collision
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	CapsuleComponent->SetGenerateOverlapEvents(true);
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnOverlapEvent);

	PickUpItemAmount = 1;

	Pickable = true;
}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemActor::LoadItemInfo(FName Name)
{
	FTableInformation* Table = DataTable->FindRow<FTableInformation>(Name, TEXT("Name"));

	ItemInformation.ItemName = Table->Name;
	ItemInformation.ItemUseGrade = Table->UseGrade;
	ItemInformation.ItemTexture = Table->Texture;
	ItemInformation.ItemIntro = Table->Intro;

}

void AItemActor::OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Pickable) {
		ItemPickUpEvent(OtherActor);
	}
}

bool AItemActor::ItemPickUpEvent(AActor* OtherActor)
{
	class UItemUserWidget* LocalItemUserWidget = NULL;
	uint16 LocalIndex = 0;
	uint8 LocalItemAmount = 0;

	DetectItemType(LocalItemUserWidget);

	//if the item can overlap
	if (ItemAllType.CanOverlap) {
		
		//if the bag has the same item
		if (ItemIsTheSame(LocalIndex, LocalItemUserWidget)) {
			//slot is full after picking up item
			if (LocalItemUserWidget->ItemBagInfo[LocalIndex].ItemAmountPickUpIsFull(PickUpItemAmount, LocalItemAmount)) {
				//if the bag has empty slot, add item
				if (ItemIsEmpty(LocalIndex, LocalItemUserWidget)) {
					LocalItemUserWidget->ItemBagInfo[LocalIndex].InventoryUserWidget->ChangeInventoryItemInfo(this->GetClass(), ItemInformation.ItemTexture, LocalItemAmount);

					LocalItemUserWidget->SetItemCurrentAmount(1);

					Destroy();
					return true;
				}
				else {
					UE_LOG(LogTemp, Warning, TEXT("YOUR BAG IS FULL"));
					return false;
				}
			}
			//slot if not full after picking up item, add item
			else {
				LocalItemUserWidget->ItemBagInfo[LocalIndex].PickUpCanOverlapItem(PickUpItemAmount);
				LocalItemUserWidget->ItemBagInfo[LocalIndex].InventoryUserWidget->ShowItemAmount();

				Destroy();
				return true;
			}
		}
		//if the bag has no same item, check for empty slot
		else {
			//if the bas has empty slot, add item
			if (ItemIsEmpty(LocalIndex, LocalItemUserWidget)) {
				LocalItemUserWidget->ItemBagInfo[LocalIndex].InventoryUserWidget->ChangeInventoryItemInfo(this->GetClass(), ItemInformation.ItemTexture, 1);

				LocalItemUserWidget->SetItemCurrentAmount(1);

				Destroy();
				return true;
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("YOUR BAG IS FULL"));
				return false;
			}
		}
	}
	else {

		//if the bag has empty slot
		if (ItemIsEmpty(LocalIndex, LocalItemUserWidget)) {
			LocalItemUserWidget->ItemBagInfo[LocalIndex].InventoryUserWidget->ChangeInventoryItemInfo(this->GetClass(), ItemInformation.ItemTexture, 1);
			
			LocalItemUserWidget->SetItemCurrentAmount(1);

			Destroy();
			return true;
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("YOUR BAG IS FULL"));
			return false;
		}	
	}
	return false;
}

void AItemActor::DetectItemType(UItemUserWidget*& ItemUserWidget)
{
	ACharacterPlayerController* Pc = Cast<ACharacterPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (Pc) {
		switch (ItemAllType.ItemType.GetValue()) {
		case E_Weapon:
			ItemUserWidget = Pc->BagUserWidget->WeaponItemUserWidget;
			break;
		case E_Consumable:
			ItemUserWidget = Pc->BagUserWidget->ConsumableItemUserWidget;
			break;
		case E_Task:
			ItemUserWidget = Pc->BagUserWidget->TaskItemUserWidget;
			break;
		}
	}
}

bool AItemActor::ItemIsEmpty(uint16& Index, UItemUserWidget* ItemBag)
{
	for (int i = 0; i < ItemBag->ItemBagInfo.Num(); i++) {
		//if bag is empty
		if (ItemBag->ItemBagInfo[i].InventoryItemClassIsEmpty()) {
			Index = i;
			return true;
		}
	}
	return false;
}

bool AItemActor::ItemIsTheSame(uint16& Index, UItemUserWidget* ItemBag)
{
	for (int i = 0; i < ItemBag->ItemBagInfo.Num(); i++) {
		
		//if the class is the same
		if (ItemBag->ItemBagInfo[i].ItemClassIsTheSame(this->GetClass())) {
			
			// if the slot is full
			if (ItemBag->ItemBagInfo[i].ItemSlotIsFull()) {
				Index = i;
				return true;
			}
			
		}
	}
	return false;
}

bool AItemActor::UseItem()
{
	//get character object
	//BaseCharacter = Cast<ARPG_Souls_likeCharacter>(UGameplayStatics::GetPlayerCharacter(GEngine->GetWorldContexts()[0].World(), 0));
	//BaseCharacter->SpawnWeapon();
	/*
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (WeaponClass) {
		FTransform WeaponTransform;
		WeaponTransform.SetLocation(FVector::ZeroVector);
		WeaponTransform.SetRotation(FQuat(FRotator::ZeroRotator));

		Weapon = GEngine->GetWorldContexts()[0].World()->SpawnActor<AWeaponItemActor>(WeaponClass, WeaponTransform, SpawnParams);

		if (Weapon) {
			Weapon->AttachToComponent(BaseCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("s_hand_r"));
			Weapon->MeshComponent->SetRelativeLocation(FVector(0, 0, 0));
		}
	}
	*/
	return false;
}

void AItemActor::ReduceUsedBagSpace()
{
//	class UItemUserWidget* LocalItemUserWidget = NULL;
//	DetectItemType(LocalItemUserWidget);
//
//	UE_LOG(LogTemp, Warning, TEXT("setitemcurrentamout"));
//	//LocalItemUserWidget->SetItemCurrentAmount(1);
}


// Called every frame
//void AItemActor::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

