// Fill out your copyright notice in the Description page of Project Settings.

#include "AssaultRifle.h"

AAssaultRifle::AAssaultRifle()
{
	StorageSlot = EInventorySlot::Secondary; 
	RifleAttachPoint = TEXT("RifleSocket"); 
	
	//GetWeaponMesh()->AddLocalRotation(FRotator(0, 0, -90)); 

	bIsActive = true;
}

void AAssaultRifle::OnEquipFinished()
{
	Super::OnEquipFinished();
	
	if (Role == ROLE_Authority) { 
		bIsActive = true;
		
		UpdateRifle(bIsActive); 
	}
}

void AAssaultRifle::OnUnEquip()
{
	Super::OnUnEquip(); 
	if (Role == ROLE_Authority) { 
		bIsActive = false; 
		
		UpdateRifle(bIsActive); 
	}
}

void AAssaultRifle::OnEnterInventory(ATyranCharacter * NewOwner)
{
	if (Role == ROLE_Authority) { 
		bIsActive = false; 
		
		UpdateRifle(bIsActive); 
	}
}

void AAssaultRifle::UpdateRifle(bool Enabled)
{
}
