// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/item/Weapon.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API AShotgun : public AWeapon
{
	GENERATED_BODY()
	
public:
	AShotgun();

	virtual bool CanFire() const override;
	virtual void HandleFiring() override;
	virtual void FireWeapon() override;

protected:
	virtual void UpdateSpreadVector() override;
	
};
