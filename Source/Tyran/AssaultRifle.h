// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "AssaultRifle.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API AAssaultRifle : public AWeapon
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly) 
	FName RifleAttachPoint;
	
	UPROPERTY(Transient/*, ReplicatedUsing = OnRep_IsActive*/)
	bool bIsActive;
	
public:
	AAssaultRifle();	

	//void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override; 
	virtual void OnEquipFinished() override; 
	virtual void OnUnEquip() override; 
	virtual void OnEnterInventory(ATyranCharacter* NewOwner) override; 
	void UpdateRifle(bool Enabled); 
	
	/*UFUNCTION() 		
	void OnRep_IsActive();*/
};
