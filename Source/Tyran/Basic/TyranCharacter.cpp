// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "TyranCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Gameplay/item/Weapon.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Basic/TyranController.h"
#include <EngineUtils.h>
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Components/LightComponent.h"
#include <DrawDebugHelpers.h>
#include "Gameplay/item/WeaponLoot.h"
#include "Components/StaticMeshComponent.h"

//////////////////////////////////////////////////////////////////////////
// ATyranCharacter
ATyranCharacter::ATyranCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	alignement = EAlignement::A_REVOLUTIONNAIRE;
	// Don't rotate when the controller rotates. Let that just affect the camera.
	//bUseControllerRotationPitch = false;
	//bUseControllerRotationYaw = false;
	//bUseControllerRotationRoll = false;

	// Configure character movement
	//GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 400.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Permettre le crouching 
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->SocketOffset = FVector(0, 35, 0);
	CameraBoom->TargetOffset = FVector(0, 0, 55);

	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	/* Noms des points d'attache tels que spécifiés dans le squelette du personnage */ 
	WeaponAttachPoint = TEXT("WeaponSocket"); 
	PelvisAttachPoint = TEXT("PelvisSocket"); 
	SpineAttachPoint = TEXT("SpineSocket");

	bWantsToFire = false;

	isVisible = false;

	isAlwaysVisible = false;

	timeBeforeDisapear = 5;

	Health = 100;
	isDead = false;

	MaxUseDistance = 600;
	DropItemDistance = 100;

	Ammunition.Add(EAmmoType::AssaultRifle, 120);
	Ammunition.Add(EAmmoType::Pistol, 0);
	Ammunition.Add(EAmmoType::Shotgun, 0);
	Ammunition.Add(EAmmoType::SniperRifle, 0);

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

	InputComponent->BindAction("Aim", IE_Pressed, this, &ATyranCharacter::OnStartAim);
	InputComponent->BindAction("Aim", IE_Released, this, &ATyranCharacter::OnStopAim);

	InputComponent->BindAction("EquipPrimaryWeapon", IE_Pressed, this, &ATyranCharacter::OnEquipPrimaryWeapon);
	InputComponent->BindAction("EquipSecondaryWeapon", IE_Pressed, this, &ATyranCharacter::OnEquipSecondaryWeapon);

	InputComponent->BindAction("Use", IE_Pressed, this, &ATyranCharacter::Use);

	InputComponent->BindAction("Reload", IE_Pressed, this, &ATyranCharacter::Reload);

	InputComponent->BindAction("NextWeapon", IE_Pressed, this, &ATyranCharacter::OnNextWeapon);
	InputComponent->BindAction("PrevWeapon", IE_Pressed, this, &ATyranCharacter::OnPrevWeapon);

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

float ATyranCharacter::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	// Call the base class - this will tell us how much damage to apply  
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f && !isDead)
	{
		/*if (Role == ROLE_Authority) {
			PlayAnimMontage(HitAnim, 1.0f, NAME_None);
		}
		else {*/
			MulticastPlayAnim(HitAnim);
		//}
		float Duration = 

		Health -= ActualDamage;
		// If the damage depletes our health set our lifespan to zero - which will destroy the actor  
		if (Health <= 0.f)
		{
			OnDeath();
		}
	}

	return ActualDamage;
}


ALoot * ATyranCharacter::GetLootInView()
{
	FVector CamLoc;
	FRotator CamRot; 
	
	if (Controller == NULL) 
		return NULL;
	
	Controller->GetPlayerViewPoint(CamLoc, CamRot); 
	const FVector TraceStart = CamLoc; 
	const FVector Direction = CamRot.Vector(); 
	const FVector TraceEnd = TraceStart + (Direction * MaxUseDistance); 
	
	FCollisionQueryParams TraceParams(FName(TEXT("TraceLoot")), true, this); 
	TraceParams.bTraceAsyncScene = true; 
	TraceParams.bReturnPhysicalMaterial = false; 
	TraceParams.bTraceComplex = false; 
	//TraceParams.AddIgnoredActor(this);
	
	FHitResult Hit(ForceInit);
	bool succes = GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);
	
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f); 
	
	return Cast<ALoot>(Hit.GetActor());
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
		// pas implémenté. 
		return ""; 
	}
}

void ATyranCharacter::AddWeapon(class AWeapon* Weapon)
{
	if (Weapon && Role == ROLE_Authority) { 
		Weapon->OnEnterInventory(this); 
		Inventory.AddUnique(Weapon); 

		// Le premier item est équipé – en main
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
	
	// Déséquipper l'arme courante 
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
	/* Nous ajouterons ici divers test permettant de déterminer si le personnage peut tirer */ 
	return result;
}

void ATyranCharacter::OnNextWeapon()
{
	if (Inventory.Num() >= 2) { 
		const int32 CurrentWeaponIndex = Inventory.IndexOfByKey(CurrentWeapon);
		AWeapon* NextWeapon = Inventory[(CurrentWeaponIndex + 1) % Inventory.Num()];
		EquipWeapon(NextWeapon); 
	}
}

void ATyranCharacter::OnPrevWeapon()
{
	if (Inventory.Num() >= 2) { 
		const int32 CurrentWeaponIndex = Inventory.IndexOfByKey(CurrentWeapon); 
		AWeapon* PrevWeapon = Inventory[(CurrentWeaponIndex - 1 + Inventory.Num()) % Inventory.Num()]; 
		EquipWeapon(PrevWeapon);
	}
}

void ATyranCharacter::OnEquipPrimaryWeapon()
{
	if (Inventory.Num() >= 1) { 
		/* Trouver l'arme qui utilise l'emplacement principal */ 
		for (int32 i = 0; i < Inventory.Num(); i++) { 
			AWeapon* Weapon = Inventory[i];

			if (Weapon->GetStorageSlot() == EInventorySlot::Primary) { 
				EquipWeapon(Weapon); 
			}
		}
	}
}

void ATyranCharacter::OnEquipSecondaryWeapon()
{
	if (Inventory.Num() >= 2) { 
		/* Trouver l'arme qui utilise l'emplacement secondaire. */ 
		for (int32 i = 0; i < Inventory.Num(); i++) {
			AWeapon* Weapon = Inventory[i];
			
			if (Weapon->GetStorageSlot() == EInventorySlot::Secondary) { 
				EquipWeapon(Weapon); 
			}
		}
	}
}


void ATyranCharacter::DropWeapon()
{
	if (Role < ROLE_Authority) { 
		ServerDropWeapon(); 
		return; 
	} 
	
	if (CurrentWeapon) { 
		FVector CamLoc; 
		FRotator CamRot; 
		if (Controller == nullptr) 
			return; 
		
		/* Déterminer un emplacement pour lacher l'arme, un peu en avant du joueur. */ 
		Controller->GetPlayerViewPoint(CamLoc, CamRot); 
		const FVector Direction = CamRot.Vector(); 
		const FVector SpawnLocation = GetActorLocation() + (Direction * DropItemDistance); 
		FActorSpawnParameters SpawnInfo; 
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; 
		
		AWeaponLoot* NewWeaponLoot = GetWorld()->SpawnActor<AWeaponLoot>(CurrentWeapon->WeaponLootClass, SpawnLocation, FRotator::ZeroRotator, SpawnInfo); 
		
		/* Lui appliquer une petite force pour que l'arme tourne un peu lorsque lachee. */ 
		UStaticMeshComponent* MeshComp = NewWeaponLoot->GetMeshComponent();
		if (MeshComp) { 
			MeshComp->AddTorque(FVector(1, 1, 1) * 4000000); 
		} 
		
		RemoveWeapon(CurrentWeapon); 
	}
}

void ATyranCharacter::RemoveWeapon(AWeapon * Weapon)
{
	if (Weapon && Role == ROLE_Authority) { 
		bool bIsCurrent = CurrentWeapon == Weapon; 
		
		if (Inventory.Contains(Weapon)) { 
			Weapon->OnLeaveInventory();
		} 
		Inventory.RemoveSingle(Weapon); 
		
		/* Replacer l'arme dans l'inventaire s'il s'agit de l'arme en main */ 
		if (bIsCurrent && Inventory.Num() > 0) 
			SetCurrentWeapon(Inventory[0]); 
		
		/* Eliminer la référence à l'objet si nous avons un inventaire vide */ 
		if (Inventory.Num() == 0) 
			SetCurrentWeapon(nullptr); 
	}
}

bool ATyranCharacter::ServerUse_Validate() { 
	return true; 
}

void ATyranCharacter::ServerUse_Implementation() { 
	Use(); 
}

bool ATyranCharacter::ServerDropWeapon_Validate() {
	return true;
}

void ATyranCharacter::ServerDropWeapon_Implementation() {
	DropWeapon();
}

bool ATyranCharacter::MulticastPlayAnim_Validate(UAnimMontage* Anim) {
	return true;
}

void ATyranCharacter::MulticastPlayAnim_Implementation(UAnimMontage* Anim) {
	PlayAnimMontage(Anim);
}

bool ATyranCharacter::MulticastStopAnim_Validate(UAnimMontage* Anim) {
	return true;
}

void ATyranCharacter::MulticastStopAnim_Implementation(UAnimMontage* Anim) {
	StopAnimMontage(Anim);
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
	// si le personnage est accroupi, il se relève avant de sauter
	if (CrouchButtonDown)
	{
		OnCrouchToggle();
	}
	else
	{
		bPressedJump = true;
		JumpButtonDown = true;
	}
}

void ATyranCharacter::OnStopJump()
{
	bPressedJump = false;
	JumpButtonDown = false;
}

void ATyranCharacter::OnCrouchToggle()
{
	if (CrouchButtonDown == false)
	{
		CrouchButtonDown = true;
		Crouch();

	}
	else
	{
		CrouchButtonDown = false;
		UnCrouch();
	}
	// Si nous sommes sur un client
	if (Role < ROLE_Authority)
	{
		ServerCrouchToggle(true); // le param n'a pas d'importance pour l'instant

	}
}

void ATyranCharacter::OnStartAim()
{
	isAiming = true;
	if (Role < ROLE_Authority)
	{
		ServerOnStartAim();
	}
}

void ATyranCharacter::OnStopAim()
{
	isAiming = false;
	if (Role < ROLE_Authority)
	{
		ServerOnStopAim();
	}
}

void ATyranCharacter::Use()
{
	if (Role == ROLE_Authority) {
		ALoot* Loot = GetLootInView();
		if (Loot) {
			Loot->OnUsed(this);
		}
	}
	else
	{
		ServerUse();
	}
}

void ATyranCharacter::Reload()
{
	if (Ammunition[CurrentWeapon->GetAmmoType()] > 0)
	{
		//if (Role == ROLE_Authority) {
			CurrentWeapon->OnReload();
		/*}
		else {
			ServerReload();
		}*/	
	}
}

void ATyranCharacter::OnDeath()
{
	isDead = true;
	MulticastStopAnim(HitAnim);
	while (Inventory.Num() > 0)
	{
		DropWeapon();
	}
	DetachFromControllerPendingDestroy();
	SetLifeSpan(10.0f);
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

		if (isAiming)
			Value /= 2.0f;

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

		if (isAiming)
			Value /= 2.0f;

		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

bool ATyranCharacter::ServerCrouchToggle_Validate(bool NewCrouching)
{
	return true;
}

void ATyranCharacter::ServerCrouchToggle_Implementation(bool NewCrouching)
{
	OnCrouchToggle();
}

bool ATyranCharacter::ServerOnStartAim_Validate()
{
	return true;
}

void ATyranCharacter::ServerOnStartAim_Implementation()
{
	OnStartAim();
}

bool ATyranCharacter::ServerOnStopAim_Validate()
{
	return true;
}

void ATyranCharacter::ServerOnStopAim_Implementation()
{
	OnStopAim();
}

bool ATyranCharacter::ServerEquipWeapon_Validate(AWeapon* Weapon) { 
	return true; 
} 

void ATyranCharacter::ServerEquipWeapon_Implementation(AWeapon* Weapon) { 
	EquipWeapon(Weapon); 
}

bool ATyranCharacter::ServerReload_Validate() {
	return true;
}

void ATyranCharacter::ServerReload_Implementation() {
	Reload();
}

void ATyranCharacter::OnRep_CurrentWeapon(AWeapon* LastWeapon) {
	SetCurrentWeapon(CurrentWeapon, LastWeapon);
}

void ATyranCharacter::OnRep_CrouchButtonDown()
{
	if (CrouchButtonDown == true)
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

void ATyranCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATyranCharacter, isVisible);
	DOREPLIFETIME(ATyranCharacter, Inventory);
	DOREPLIFETIME(ATyranCharacter, CurrentWeapon);
	DOREPLIFETIME(ATyranCharacter, Health);
	DOREPLIFETIME(ATyranCharacter, isDead);
	DOREPLIFETIME(ATyranCharacter, isAiming);
	DOREPLIFETIME_CONDITION(ATyranCharacter, CrouchButtonDown, COND_SkipOwner);
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

bool ATyranCharacter::WeaponSlotAvailable(EInventorySlot CheckSlot)
{
	/* Fonction de recherche spéciale qui nous évite de tester chaque élément de l'inventaire. */ 
	return nullptr == Inventory.FindByPredicate([=](AWeapon* W) { 
		return W->GetStorageSlot() == CheckSlot;
	});
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

	if (Controller && Controller->IsLocalController()) {
		ALoot* Loot = GetLootInView();
		
		// Terminer le focus sur l'objet précédent 
		if (FocusedLoot != Loot) {
			if (FocusedLoot) {
				FocusedLoot->OnEndFocus();
			} 

			bHasNewFocus = true; 
		} 
		
		// Assigner le nouveau focus (peut être nul ) 
		FocusedLoot = Loot;
		
		// Démarrer un nouveau focus si Usable != null; 
		if (Loot) {
			if (bHasNewFocus) { 
				Loot->OnBeginFocus();
				bHasNewFocus = false; 
				
				// Pour débogage, vous pourrez l'oter par la suite 
				// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Focus")); 
			} 
		}
	}
}