// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/item/Weapon.h"
#include "Pistol.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API APistol : public AWeapon
{
	GENERATED_BODY()
	
public:
	APistol();

	virtual bool CanFire() const override;
	virtual void HandleFiring() override;
	virtual void FireWeapon() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	
};
