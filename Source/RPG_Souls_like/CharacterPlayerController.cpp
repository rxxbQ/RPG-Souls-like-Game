// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPlayerController.h"
#include "MyAnimInstance.h"
#include "RPG_Souls_likeCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BagUserWidget.h"
#include "GamePlayerUserWidget/CharacterAttributeUserWidget.h"

ACharacterPlayerController::ACharacterPlayerController() 
{
	bShowMouseCursor = false;
}

void ACharacterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//create bag widget
	BagUserWidget = CreateWidget<UBagUserWidget>(GetGameInstance(), LoadClass<UBagUserWidget>(NULL, TEXT("WidgetBlueprint'/Game/UI/BagUserWidget.BagUserWidget_C'")));

	//create attribute widget
	AttributeUserWidget = CreateWidget<UCharacterAttributeUserWidget>(GetGameInstance(), 
		LoadClass<UCharacterAttributeUserWidget>(NULL, TEXT("WidgetBlueprint'/Game/UI/CharacterAttributesUI.CharacterAttributesUI_C'")));
}

void ACharacterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("CharacterMoveType", IE_Pressed, this, &ACharacterPlayerController::CharacterMoveTypeChange);
	InputComponent->BindAction("OpenBag", IE_Pressed, this, &ACharacterPlayerController::OpenBagEvent);
	InputComponent->BindAction("ShowMouse", IE_Pressed, this, &ACharacterPlayerController::ShowMouseEvent);

	InputComponent->BindAction("AttributeUI", IE_Pressed, this, &ACharacterPlayerController::OpenCharacterAttributeUI);
}

void ACharacterPlayerController::CharacterMoveTypeChange()
{
	ARPG_Souls_likeCharacter* Ch = Cast<ARPG_Souls_likeCharacter>(GetCharacter());
	if (Ch) {
		UMyAnimInstance* AnimInstance = Cast<UMyAnimInstance>(Ch->GetMesh()->GetAnimInstance());
		if (AnimInstance) {
			switch (AnimInstance->MoveSpeed) {
			case 0:
				AnimInstance->MoveSpeed = 2;
				Ch->GetCharacterMovement()->MaxWalkSpeed = 700;
				break;
			
			case 2:
				AnimInstance->MoveSpeed = 0;
				Ch->GetCharacterMovement()->MaxWalkSpeed = 300;
				break;
			}

		}
	}
}

void ACharacterPlayerController::OpenBagEvent()
{
	//open and close bag
	BagUserWidget->IsInViewport() ? BagUserWidget->RemoveFromParent() : BagUserWidget->AddToViewport() ;
}

void ACharacterPlayerController::ShowMouseEvent()
{
	if (bShowMouseCursor) {
		bShowMouseCursor = false;
	}
	else {
		bShowMouseCursor = true;
	}
}

void ACharacterPlayerController::OpenCharacterAttributeUI()
{
	//open and close status ui
	AttributeUserWidget->IsInViewport() ? AttributeUserWidget->RemoveFromParent() : AttributeUserWidget->AddToViewport();
}
