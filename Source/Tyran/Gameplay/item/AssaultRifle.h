// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Weapon.h"
#include "AssaultRifle.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API AAssaultRifle : public AWeapon
{
	GENERATED_BODY()

	//UPROPERTY(EditDefaultsOnly) 
	//FName RifleAttachPoint;
	
public:
	AAssaultRifle();	

	virtual void FireWeapon() override;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override; 
};
