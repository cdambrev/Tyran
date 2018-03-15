// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"

// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh3P"));
	Mesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	Mesh->bReceivesDecals = true;
	Mesh->CastShadow = true;
	Mesh->SetCollisionObjectType(ECC_WorldDynamic);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	Mesh->SetSimulatePhysics(false);
	RootComponent = Mesh;

	StorageSlot = EInventorySlot::Primary;

	bIsEquipped = false;
	bPendingEquip = false;

	// Pour multi-joueurs 
	bReplicates = true;
	bReplicateMovement = true;
	bNetUseOwnerRelevancy = true;

	MuzzleAttachPoint = TEXT("MuzzleFlashSocket");
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

}

void AWeapon::OnEnterInventory(ATyranCharacter* NewOwner)
{
	SetOwningPawn(NewOwner);
	AttachMeshToPawn(StorageSlot);
}

void AWeapon::OnLeaveInventory()
{
	if (Role == ROLE_Authority) {
		SetOwningPawn(nullptr);
	}

	if (IsEquippedOrPending()) {
		OnUnEquip();
	}
	DetachMeshFromPawn();

}

bool AWeapon::IsEquippedOrPending() const
{
	return bIsEquipped || bPendingEquip;
}

void AWeapon::OnUnEquip()
{
	AttachMeshToPawn(StorageSlot);
	bIsEquipped = false;
	// StopFire(); 

	if (bPendingEquip) {
		StopWeaponAnimation(EquipAnim);
		bPendingEquip = false;

		GetWorldTimerManager().ClearTimer(EquipFinishedTimerHandle);
	}

	if (bPendingReload) {
		StopWeaponAnimation(ReloadAnim);
		bPendingReload = false;

		GetWorldTimerManager().ClearTimer(ReloadFinishedTimerHandle);
	}

	DetermineWeaponState();
}

void AWeapon::OnEquip()
{
	AttachMeshToPawn();

	bPendingEquip = true;
	DetermineWeaponState();

	float Duration = PlayWeaponAnimation(EquipAnim);
	if (Duration <= 0.0f) {
		// Pour ne pas avoir de problème 
		Duration = 0.5f;
	}
	EquipStartedTime = GetWorld()->TimeSeconds;
	EquipDuration = Duration;

	GetWorldTimerManager().SetTimer(EquipFinishedTimerHandle, this, &AWeapon::OnEquipFinished, Duration, false);
	/*if (MyPawn && MyPawn->IsLocallyControlled()) {
		PlayWeaponSound(EquipSound);
	}*/
}

void AWeapon::DetermineWeaponState()
{
	EWeaponState NewState = EWeaponState::Idle;
	if (bIsEquipped) {
		if (bWantsToFire && CanFire()) 
		{ 
			NewState = EWeaponState::Firing;
		} 
	}
	else if (bPendingEquip) {
		NewState = EWeaponState::Equipping;
	}

	SetWeaponState(NewState);
}

void AWeapon::SetWeaponState(EWeaponState NewState)
{
	const EWeaponState PrevState = CurrentState;

	if (PrevState == EWeaponState::Firing && NewState != EWeaponState::Firing) {
		OnBurstFinished(); 
	}

	CurrentState = NewState;

	if (PrevState != EWeaponState::Firing && NewState == EWeaponState::Firing) {
		OnBurstStarted(); 
	}
}

EWeaponState AWeapon::GetCurrentState() const
{
	return CurrentState;
}

bool AWeapon::IsEquipped() const
{
	return bIsEquipped;
}

void AWeapon::OnEquipFinished()
{
	AttachMeshToPawn();

	bIsEquipped = true;
	bPendingEquip = false;

	DetermineWeaponState();
}

float AWeapon::GetEquipStartedTime() const
{
	return EquipStartedTime;
}

float AWeapon::GetEquipDuration() const
{
	return EquipDuration;
}

float AWeapon::PlayWeaponAnimation(UAnimMontage* Animation, float InPlayRate /*= 1.f*/, FName StartSectionName /*= NAME_None*/)
{
	float Duration = 0.0f;
	if (MyPawn) {
		Duration = MyPawn->PlayAnimMontage(Animation, InPlayRate, StartSectionName);
	}

	return Duration;
}

void AWeapon::StopWeaponAnimation(UAnimMontage * Animation)
{
	if (MyPawn) {
		if (Animation) {
			MyPawn->StopAnimMontage(Animation);
		}
	}
}

void AWeapon::StartFire()
{
	if (Role < ROLE_Authority) {
		ServerStartFire();
	}
	if (!bWantsToFire) {
		bWantsToFire = true;
		DetermineWeaponState();
	}
}

void AWeapon::StopFire() {
	if (Role < ROLE_Authority) {
		ServerStopFire();
	}
	if (bWantsToFire) {
		bWantsToFire = false;
		DetermineWeaponState();
	}
}

bool AWeapon::ServerStartFire_Validate() {
	return true;
}

void AWeapon::ServerStartFire_Implementation() {
	StartFire();
}

bool AWeapon::ServerStopFire_Validate() {
	return true;
}

void AWeapon::ServerStopFire_Implementation() {
	StopFire();
}

void AWeapon::OnBurstStarted() {
	// Démarrer le tir, pourra être retardé pour tenir compte de TimeBetweenShots 
	const float GameTime = GetWorld()->GetTimeSeconds(); 

	if (LastFireTime > 0 && TimeBetweenShots > 0.0f && LastFireTime + TimeBetweenShots > GameTime) { 
		GetWorldTimerManager().SetTimer(HandleFiringTimerHandle, this, &AWeapon::HandleFiring, LastFireTime + TimeBetweenShots - GameTime, false); 
	} else { 
		HandleFiring(); 
	} 
} 

void AWeapon::OnBurstFinished() { 
	BurstCounter = 0; 

	if (GetNetMode() != NM_DedicatedServer) { 
		StopSimulatingWeaponFire(); 
	} 

	GetWorldTimerManager().ClearTimer(HandleFiringTimerHandle); 
	bRefiring = false; 
}

void AWeapon::HandleFiring() {
	if (CanFire()) {
		if (GetNetMode() != NM_DedicatedServer) {
			SimulateWeaponFire(); 
		} 
		if (MyPawn && MyPawn->IsLocallyControlled()) {
			FireWeapon(); 

			// Mettre à jour l'effet sur les objets distants 
			BurstCounter++; 
		} 
		MagazineCurrent--;
	} 
	if (MyPawn && MyPawn->IsLocallyControlled()) { 
		if (Role < ROLE_Authority) { 
			ServerHandleFiring(); 
		} 
		
		bRefiring = (CurrentState == EWeaponState::Firing && TimeBetweenShots > 0.0f); 
		if (bRefiring) { 
			GetWorldTimerManager().SetTimer(HandleFiringTimerHandle, this, &AWeapon::HandleFiring, TimeBetweenShots, false); 
		} 
	} 
	
	LastFireTime = GetWorld()->GetTimeSeconds(); 
}

bool AWeapon::ServerHandleFiring_Validate() { 
	return true; 
} 

void AWeapon::ServerHandleFiring_Implementation() {
	const bool bShouldUpdateAmmo = CanFire(); 
	HandleFiring(); 
	if (bShouldUpdateAmmo) { 
		// Mettre à jour l'effet sur les clients distants 
		BurstCounter++; 
	} 
}

bool AWeapon::CanFire() const { 
	bool bPawnCanFire = MyPawn && MyPawn->CanFire(); 
	bool bStateOK = CurrentState == EWeaponState::Idle || CurrentState == EWeaponState::Firing; 
	bool bNotMagEmpty = MagazineCurrent > 0;
	return bPawnCanFire && bStateOK && bNotMagEmpty; 
}

void AWeapon::SimulateWeaponFire() { 
	if (MuzzleFX) { 
		MuzzlePSC = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, Mesh, MuzzleAttachPoint); 
	} 
	
	if (!bPlayingFireAnim) { 
		PlayWeaponAnimation(FireAnim); 
		bPlayingFireAnim = true; 
	} 
	//PlayWeaponSound(FireSound); 
}

void AWeapon::StopSimulatingWeaponFire() {
	if (bPlayingFireAnim) {
		StopWeaponAnimation(FireAnim);
		bPlayingFireAnim = false;
	}
}

FVector AWeapon::GetMuzzleLocation() const {
	return Mesh->GetSocketLocation(MuzzleAttachPoint); 
} 

FVector AWeapon::GetMuzzleDirection() const {
	return Mesh->GetSocketRotation(MuzzleAttachPoint).Vector();
}

EAmmoType AWeapon::GetAmmoType()
{
	return AmmoType;
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::SetOwningPawn(ATyranCharacter * NewOwner)
{
	if (MyPawn != NewOwner) {
		Instigator = NewOwner;
		MyPawn = NewOwner;
		// Propriétaire réseau pour les appels RPC. 
		SetOwner(NewOwner);
	}
}

void AWeapon::AttachMeshToPawn(EInventorySlot Slot)
{
	if (MyPawn) {
		// Retirer et cacher 
		DetachMeshFromPawn();

		USkeletalMeshComponent* PawnMesh = MyPawn->GetMesh();
		FName AttachPoint = MyPawn->GetInventoryAttachPoint(Slot);
		Mesh->SetHiddenInGame(false);
		// On le rend visible 
		Mesh->AttachToComponent(PawnMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, AttachPoint);
	}
}

void AWeapon::DetachMeshFromPawn()
{
	Mesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	Mesh->SetHiddenInGame(true);
}

USkeletalMeshComponent* AWeapon::GetWeaponMesh() const
{
	return Mesh;
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, MyPawn);
	DOREPLIFETIME(AWeapon, MagazineCurrent);
	DOREPLIFETIME(AWeapon, bPendingReload);
}

void AWeapon::OnRep_MyPawn() {
	if (MyPawn) {
		OnEnterInventory(MyPawn);
	}
	else {
		OnLeaveInventory();
	}
}

void AWeapon::OnReload()
{
	if (MyPawn->Ammunition[AmmoType] > 0 && MagazineCurrent < MagazineSize)
	{
		bPendingReload = true;

		float Duration = PlayWeaponAnimation(ReloadAnim);
		if (Duration <= 0.0f) {
			// Pour ne pas avoir de problème 
			Duration = 0.5f;
		}
		ReloadStartedTime = GetWorld()->TimeSeconds;
		ReloadDuration = Duration;

		GetWorldTimerManager().SetTimer(ReloadFinishedTimerHandle, this, &AWeapon::OnReloadFinished, Duration, false);
	}
}

bool AWeapon::ServerReload_Validate() {
	return true;
}

void AWeapon::ServerReload_Implementation() {
	OnReload();
}

void AWeapon::OnReloadFinished()
{
	bPendingReload = false;

	int NbAmmoToFill = MagazineSize - MagazineCurrent;
	if (MyPawn->Ammunition[AmmoType] > NbAmmoToFill)
	{
		MagazineCurrent = MagazineSize;
		MyPawn->Ammunition[AmmoType] -= NbAmmoToFill;
	} 
	else
	{
		MagazineCurrent += MyPawn->Ammunition[AmmoType];
		MyPawn->Ammunition[AmmoType] = 0;
	}

	DetermineWeaponState();
}

void AWeapon::OnRep_BurstCounter() {
	if (BurstCounter > 0) { 
		SimulateWeaponFire(); 
	} else { 
		StopSimulatingWeaponFire(); 
	} 
}

void AWeapon::OnRep_bPendingReload()
{
	if (bPendingReload)
	{
		PlayWeaponAnimation(ReloadAnim);
		//OnReload();
	}
	/*else
	{
		OnReloadFinished();
	}*/
}