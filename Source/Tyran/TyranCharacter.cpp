// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "TyranCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Weapon.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "TyranController.h"
//#include "ManagerViewPawn.h"
#include <EngineUtils.h>
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Components/LightComponent.h"

//////////////////////////////////////////////////////////////////////////
// ATyranCharacter
ATyranCharacter::ATyranCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

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
	GetCharacterMovement()->JumpZVelocity = 400.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Permettre le crouching 
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

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

	/* Noms des points d'attache tels que sp�cifi�s dans le squelette du personnage */ 
	WeaponAttachPoint = TEXT("WeaponSocket"); 
	PelvisAttachPoint = TEXT("PelvisSocket"); 
	SpineAttachPoint = TEXT("SpineSocket");

	bWantsToFire = false;

	isVisible = false;

	isAlwaysVisible = false;

	timeBeforeDisapear = 5;

	PrimaryActorTick.bCanEverTick = true;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATyranCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATyranCharacter::OnStartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ATyranCharacter::OnStopJump);

	InputComponent->BindAction("CrouchToggle", IE_Released, this, &ATyranCharacter::OnCrouchToggle);

	InputComponent->BindAction("Fire", IE_Pressed, this, &ATyranCharacter::OnStartFire);
	InputComponent->BindAction("Fire", IE_Released, this, &ATyranCharacter::OnStopFire);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATyranCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATyranCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATyranCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATyranCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ATyranCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ATyranCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ATyranCharacter::OnResetVR);
}


FName ATyranCharacter::GetInventoryAttachPoint(EInventorySlot Slot) const
{
	/* Retourne le nom du socket */ 
	switch (Slot) {
	case EInventorySlot::Hands: 
		return WeaponAttachPoint; 
	case EInventorySlot::Primary: 
		return SpineAttachPoint; 
	case EInventorySlot::Secondary:
		return PelvisAttachPoint;
	default: 
		// pas impl�ment�. 
		return ""; 
	}
}

void ATyranCharacter::AddWeapon(class AWeapon* Weapon)
{
	if (Weapon && Role == ROLE_Authority) { 
		Weapon->OnEnterInventory(this); 
		Inventory.AddUnique(Weapon); 

		// Le premier item est �quip� � en main
		if (Inventory.Num() > 0) { 
			EquipWeapon(Inventory[0]); 
		} 
	}
}

void ATyranCharacter::EquipWeapon(AWeapon * Weapon)
{
	if (Weapon) {
		if (Role == ROLE_Authority) { 
			SetCurrentWeapon(Weapon); 
		} else { 
			ServerEquipWeapon(Weapon); 
		} 
	}
}

void ATyranCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents(); 
	if (Role == ROLE_Authority) { 
		SpawnDefaultInventory(); 
	}
}

void ATyranCharacter::SpawnDefaultInventory()
{
	if (Role < ROLE_Authority) { 
		return;
	} 
	
	for (int32 i = 0; i < DefaultInventoryClasses.Num(); i++) { 
		if (DefaultInventoryClasses[i]) {
			FActorSpawnParameters SpawnInfo; 
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; 
			AWeapon* NewWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultInventoryClasses[i], SpawnInfo); 

			AddWeapon(NewWeapon); 
		} 
	}
}

void ATyranCharacter::SetCurrentWeapon(AWeapon * NewWeapon, AWeapon * LastWeapon)
{
	AWeapon* LocalLastWeapon = nullptr; 

	if (LastWeapon) { 
		LocalLastWeapon = LastWeapon; 
	}
	else if (NewWeapon != CurrentWeapon) { 
		LocalLastWeapon = CurrentWeapon; 
	} 
	
	// D�s�quipper l'arme courante 
	if (LocalLastWeapon) { 
		LocalLastWeapon->OnUnEquip(); 
	}
	
	CurrentWeapon = NewWeapon; 
	
	if (NewWeapon) { 
		NewWeapon->SetOwningPawn(this); 
		NewWeapon->OnEquip(); 
	}
}

void ATyranCharacter::OnStartFire()
{
	StartWeaponFire();
}

void ATyranCharacter::OnStopFire()
{
	StopWeaponFire();
}

void ATyranCharacter::StartWeaponFire()
{
	if (!bWantsToFire) { 
		bWantsToFire = true; 
		if (CurrentWeapon) { 
			CurrentWeapon->StartFire(); 
		} 
	}
}

void ATyranCharacter::StopWeaponFire()
{
	if (bWantsToFire) { 
		bWantsToFire = false; 
		if (CurrentWeapon) { 
			CurrentWeapon->StopFire(); 
		} 
	}
}

bool ATyranCharacter::CanFire() const
{
	bool result = true; 
	/* Nous ajouterons ici divers test permettant de d�terminer si le personnage peut tirer */ 
	return result;
}

void ATyranCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ATyranCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ATyranCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ATyranCharacter::OnStartJump()
{
	// si le personnage est accroupi, il se rel�ve avant de sauter 
	if (bIsCrouched) { 
		CrouchButtonDown = false; 
		UnCrouch(); 
	} 
	bPressedJump = true; 
	JumpButtonDown = true;
}

void ATyranCharacter::OnStopJump()
{
	bPressedJump = false;
	JumpButtonDown = false;
}

void ATyranCharacter::OnCrouchToggle()
{
	// Si nous sommes d�j� accroupis, CanCrouch retourne false. 
	if (CanCrouch()) { 
		CrouchButtonDown = true; 
		Crouch();
	} else { 
		CrouchButtonDown = false; 
		UnCrouch(); 
	}
}

void ATyranCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATyranCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATyranCharacter::MoveForward(float Value)
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

void ATyranCharacter::MoveRight(float Value)
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

bool ATyranCharacter::ServerEquipWeapon_Validate(AWeapon* Weapon) { 
	return true; 
} 

void ATyranCharacter::ServerEquipWeapon_Implementation(AWeapon* Weapon) { 
	EquipWeapon(Weapon); 
}

void ATyranCharacter::OnRep_CurrentWeapon(AWeapon* LastWeapon) {
	SetCurrentWeapon(CurrentWeapon, LastWeapon);
}

void ATyranCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATyranCharacter, isVisible);
	DOREPLIFETIME(ATyranCharacter, Inventory);
	DOREPLIFETIME(ATyranCharacter, CurrentWeapon);
}

void ATyranCharacter::setVisible(bool b) {
	isVisible = b;
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator) {
		if (static_cast<ATyranController *>(&**Iterator)->IsLocalPlayerController() && static_cast<ATyranController *>(&**Iterator)->isTyran) {
			//SetActorHiddenInGame(!b);
			GetCapsuleComponent()->SetVisibility(b, true);
		}
	}
}

EAlignement ATyranCharacter::getAlignement() {
	return alignement;
}

void ATyranCharacter::setViewedThisTick()
{
	timeSinceLastView = 0;
}

void ATyranCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!isAlwaysVisible) {
		if (timeSinceLastView < timeBeforeDisapear) {
			++timeSinceLastView;
			if (timeSinceLastView >= timeBeforeDisapear) {
				setVisible(false);
			}
			else {
				setVisible(true);
			}
		}
	}
}