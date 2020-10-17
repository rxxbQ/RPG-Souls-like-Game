// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPG_Souls_likeCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Inventory/WeaponItemActor.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "AI/AITags.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

//////////////////////////////////////////////////////////////////////////
// ARPG_Souls_likeCharacter

ARPG_Souls_likeCharacter::ARPG_Souls_likeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(38.f, 83.0f);

	//initial location and rotation
	GetMesh()->SetRelativeLocation(FVector(0, -10, -83));
	GetMesh()->SetRelativeRotation(FRotator(0, 0, -90));

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	//initial movement speed
	GetCharacterMovement()->MaxWalkSpeed = 300;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	SetupStimulus();

}

void ARPG_Souls_likeCharacter::BeginPlay()
{
	Super::BeginPlay();

	ARPG_Souls_likeCharacter::SpawnWeapon();

	Weapon->WeaponCollisionBox->OnComponentHit.AddDynamic(this, &ARPG_Souls_likeCharacter::OnAttackHit);

	//Weapon->WeaponCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ARPG_Souls_likeCharacter::OnAttackOverlapBegin);
	//Weapon->WeaponCollisionBox->OnComponentEndOverlap.AddDynamic(this, &ARPG_Souls_likeCharacter::OnAttackOverlapEnd);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARPG_Souls_likeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARPG_Souls_likeCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARPG_Souls_likeCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ARPG_Souls_likeCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ARPG_Souls_likeCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ARPG_Souls_likeCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ARPG_Souls_likeCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ARPG_Souls_likeCharacter::OnResetVR);

	// attack functionality
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ARPG_Souls_likeCharacter::AttackInput);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &ARPG_Souls_likeCharacter::AttackEnd);

	//PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ARPG_Souls_likeCharacter::SpawnWeapon);

	PlayerInputComponent->BindAction("Distract", IE_Pressed, this, &ARPG_Souls_likeCharacter::OnDistract);

	
}

void ARPG_Souls_likeCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ARPG_Souls_likeCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ARPG_Souls_likeCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ARPG_Souls_likeCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARPG_Souls_likeCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ARPG_Souls_likeCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ARPG_Souls_likeCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ARPG_Souls_likeCharacter::AttackStart()
{
	Weapon->WeaponCollisionBox->SetCollisionProfileName("Weapon");
	Weapon->WeaponCollisionBox->SetNotifyRigidBodyCollision(true);
	//Weapon->WeaponCollisionBox->SetGenerateOverlapEvents(true);

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *Weapon->WeaponCollisionBox->GetCollisionProfileName().ToString());
}

void ARPG_Souls_likeCharacter::AttackEnd()
{
	Weapon->WeaponCollisionBox->SetCollisionProfileName("NoCollision");
	Weapon->WeaponCollisionBox->SetNotifyRigidBodyCollision(false);
	//Weapon->WeaponCollisionBox->SetGenerateOverlapEvents(false);

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *Weapon->WeaponCollisionBox->GetCollisionProfileName().ToString());
}

void ARPG_Souls_likeCharacter::AttackInput()
{

}

void ARPG_Souls_likeCharacter::OnAttackHit(UPrimitiveComponent* HitComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("hit: %s"), *Hit.GetActor()->GetName());
}

void ARPG_Souls_likeCharacter::OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("overlap begin: %s"), *OtherActor->GetName());
}

void ARPG_Souls_likeCharacter::OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("overlap end: %s"), *OtherActor->GetName());
}


FText ARPG_Souls_likeCharacter::GetCharacterClass()
{
	switch (GetCharacterProperty().CharacterClass.GetValue()) {
	case Assasin:
		return FText::FromString(TEXT("Class : CAssasin"));
		break;

	case Mage:
		return FText::FromString(TEXT("Class : Mage"));
		break;

	case Warrior:
		return FText::FromString(TEXT("Class : Warrior"));
		break;

	case Knight:
		return FText::FromString(TEXT("Class : Knight"));
		break;

	case Deprived:
		return FText::FromString(TEXT("Class : Deprived"));
		break;

	}
	
	return FText();
}

FText ARPG_Souls_likeCharacter::GetCharacterBasic(int32 Max, int32 Current, FString String)
{
	FString NewString = String+ FString::FromInt(Current) + TEXT(" / ") + FString::FString::FromInt(Max);
	
	return FText::FromString(NewString);
}

FText ARPG_Souls_likeCharacter::GetCharacterAttribute(int32 SelfAttribute, FString String)
{
	int32 OutAttribute;
	int32 WeaponAttribute = 0;
	OutAttribute = SelfAttribute + WeaponAttribute;

	FString NewString = String + FString::FromInt(SelfAttribute) + TEXT(" Weapon : ") + FString::FromInt(WeaponAttribute) + TEXT(" Total : ") + FString::FromInt(OutAttribute);
	
	return FText::FromString(NewString);
}

void ARPG_Souls_likeCharacter::SpawnWeapon()
{
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	
		if (WeaponClass) {
			FTransform WeaponTransform;
			WeaponTransform.SetLocation(FVector::ZeroVector);
			WeaponTransform.SetRotation(FQuat(FRotator::ZeroRotator));

			Weapon = GetWorld()->SpawnActor<AWeaponItemActor>(WeaponClass, WeaponTransform, SpawnParams);
			
			if (Weapon) {
				//UE_LOG(LogTemp, Warning, TEXT("%d"), Weapon->WeaponNum);
				//if (Weapon->WeaponNum != 0) {
				//	if (Weapon->WeaponNum == 1) {
				//		Weapon->WeaponOnHand = FName(TEXT("BlackKnight"));
				//	}
				//	else if (Weapon->WeaponNum == 3) {
				//		Weapon->WeaponOnHand = FName(TEXT("DragonSword"));
				//	}
				//	Weapon->SetActorEnableCollision(false);
					Weapon->SetupWeapon(FName("BlackKnight"));
					Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("s_hand_r"));
					Weapon->MeshComponent->SetRelativeLocation(FVector(0, 0, 0));
					Weapon->Pickable = false;
				//}
			}
		}
		
}

void ARPG_Souls_likeCharacter::SetupStimulus()
{
	Stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	Stimulus->RegisterWithPerceptionSystem();
}

void ARPG_Souls_likeCharacter::OnDistract()
{
	if (DistractionSound) {
		FVector const Location = GetActorLocation();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DistractionSound, Location);
		UAISense_Hearing::ReportNoiseEvent(GetWorld(), Location, 1.0f, this, 0.0f, Tags::NoiseTag);
	}
}


