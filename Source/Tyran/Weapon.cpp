// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "kismet/GameplayStatics.h"

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
	RootComponent = Mesh;

	StorageSlot = EInventorySlot::Primary;

	bIsEquipped = false; 
	bPendingEquip = false;
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
		//if (bWantsToFire && CanFire()) 
		//{ 
			// NewState = EWeaponState::Firing;
		//} 
	} else if (bPendingEquip) { 
		NewState = EWeaponState::Equipping; 
	} 
	
	SetWeaponState(NewState);
}

void AWeapon::SetWeaponState(EWeaponState NewState)
{
	const EWeaponState PrevState = CurrentState;
	
	if (PrevState == EWeaponState::Firing && NewState != EWeaponState::Firing) {
		// OnBurstFinished(); 
	}
	
	CurrentState = NewState; 
	
	if (PrevState != EWeaponState::Firing && NewState == EWeaponState::Firing) { 
		// OnBurstStarted(); 
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
		if (Animation) {
			Duration = MyPawn->PlayAnimMontage(Animation, InPlayRate, StartSectionName); 
		} 
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

